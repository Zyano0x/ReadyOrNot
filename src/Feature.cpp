#include "pch.h"

Game g_Game;

Game::Game()
{
	InitSDK();
}

Game::~Game() {}

void Game::Setup()
{
	UWorld* GWorld = UWorld::GetWorld();
	if (!UKismetSystemLibrary::IsValid(GWorld))
		return;

	UGameInstance* OwningGameInstance = GWorld->OwningGameInstance;
	if (!UKismetSystemLibrary::IsValid(OwningGameInstance))
		return;

	ULocalPlayer* LocalPlayer = OwningGameInstance->LocalPlayers[0];
	if (!LocalPlayer)
		return;

	LocalPlayerController = LocalPlayer->PlayerController;
	if (!UKismetSystemLibrary::IsValid(LocalPlayerController))
		return;

	LocalCharacter = static_cast<APlayerCharacter*>(LocalPlayerController->GetPawn());
	if (!UKismetSystemLibrary::IsValid(LocalCharacter))
		return;

	LocalPlayerCamera = LocalPlayerController->PlayerCameraManager;
	if (!UKismetSystemLibrary::IsValid(LocalPlayerCamera))
		return;

	LocalPlayerController->GetViewportSize(&m_ScreenWidth, &m_ScreenHeight);
}

void Game::Visual()
{
	if (!Settings[ESP_ENABLED].Value.bValue)
		return;

	UWorld* GWorld = UWorld::GetWorld();
	if (!UKismetSystemLibrary::IsValid(GWorld))
		return;

	if (!UKismetSystemLibrary::IsValid(LocalCharacter))
		return;

	AReadyOrNotPlayerState* LocalPlayerState = static_cast<AReadyOrNotPlayerState*>(LocalCharacter->PlayerState);
	if (!UKismetSystemLibrary::IsValid(LocalPlayerState))
		return;

	if (!LocalPlayerState->bIsInGame)
		return;

	TArray<AReadyOrNotCharacter*> Players = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllReadyOrNotCharacters;
	for (int i = 0; i < Players.Count(); i++)
	{
		AReadyOrNotCharacter* Player = Players[i];
		if (!UKismetSystemLibrary::IsValid(Player))
			continue;

		USkeletalMeshComponent* MeshComponent = Player->Mesh;
		if (!UKismetSystemLibrary::IsValid(MeshComponent))
			continue;

		if (Player->IsLocalPlayer())
			continue;

		FVector Location, Extend;
		Player->GetActorBounds(true, &Location, &Extend, false);

		bool IsTeammate = Player->IsOnSWATTeam();
		bool IsCivilian = Player->IsCivilian();

		bool ShowEnemy = !IsTeammate && !IsCivilian && Settings[ESP_ENEMY].Value.bValue;
		bool ShowFriendly = IsTeammate && !IsCivilian && Settings[ESP_FRIENDLY].Value.bValue;
		bool ShowCivilian = !IsTeammate && IsCivilian && Settings[ESP_CIVILIAN].Value.bValue;

		if (!(ShowEnemy || ShowFriendly || ShowCivilian))
			continue;

		if (ShowEnemy)
		{
			FHitResult HitResult;
			memset(&HitResult, 0, sizeof(FHitResult));
			if (UKismetSystemLibrary::LineTraceSingle(GWorld, LocalPlayerCamera->GetCameraLocation(), Location, ETraceTypeQuery::TraceTypeQuery1, EDrawDebugTrace::None, &HitResult, FLinearColor(255.0f, 255.0f, 255.0f, 255.0f), FLinearColor(255.0f, 255.0f, 255.0f, 255.0f)))
				VEC4CPY(Settings[ESP_VISIBLE_COLOR].Value.v4Value, m_Color);
			else
				VEC4CPY(Settings[ESP_ENEMY_COLOR].Value.v4Value, m_Color);
		}
		else if (ShowFriendly)
		{
			VEC4CPY(Settings[ESP_FRIENDLY_COLOR].Value.v4Value, m_Color);
		}
		else if (ShowCivilian)
		{
			VEC4CPY(Settings[ESP_CIVILIAN_COLOR].Value.v4Value, m_Color);
		}

		FVector2D HeadPos, FootPos;
		if (LocalPlayerController->ProjectWorldLocationToScreen({ Location.X, Location.Y, Location.Z - Extend.Z }, &FootPos, false) &&
			LocalPlayerController->ProjectWorldLocationToScreen({ Location.X, Location.Y, Location.Z + Extend.Z }, &HeadPos, false))
		{
			const float Distance = Player->GetDistanceTo(LocalCharacter) / 100.0f;
			const float Height = abs(HeadPos.Y - FootPos.Y) / 0.6f;
			const float Width = Height * 0.4f;

			FVector2D Top = { HeadPos.X - Width * 0.5f, HeadPos.Y };
			FVector2D Bottom = { HeadPos.X + Width * 0.5f, FootPos.Y };

			if (Settings[ESP_NAME].Value.bValue)
			{
				if (ShowCivilian)
				{
					Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("[Civilian]")), (Top.X + Bottom.X) / 2, Top.Y - 20, 15.0f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
				}
				else if (ShowEnemy)
				{
					Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("[Suspect]")), (Top.X + Bottom.X) / 2, Top.Y - 20, 15.0f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
				}
				else if (ShowFriendly)
				{
					Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("[Teammate]")), (Top.X + Bottom.X) / 2, Top.Y - 20, 15.0f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
				}
			}

			if (Settings[ESP_SKELETON].Value.bValue)
			{
				std::vector<std::pair<const char*, const char*>> SkeletonConnections = {
						{ "head_equipment", "Head" },
						{ "Head", "ik_hand_spine_root" },
						{ "ik_hand_spine_root", "torso_stabilizer" },
						{ "torso_stabilizer", "ik_hand_spine_root" },
						{ "ik_hand_spine_root", "Head" },

						{ "torso_stabilizer", "thigh_LE" },
						{ "thigh_LE", "calf_LE" },
						{ "calf_LE", "foot_LE" },
						{ "foot_LE", "ball_LE" },

						{ "torso_stabilizer", "thigh_RI" },
						{ "thigh_RI", "calf_RI" },
						{ "calf_RI", "foot_RI" },
						{ "foot_RI", "ball_RI" },

						{ "ik_hand_spine_root", "clavicle_LE" },
						{ "clavicle_LE", "upperarm_LE" },
						{ "upperarm_LE", "lowerarm_LE" },
						{ "lowerarm_LE", "hand_LE" },

						{ "ik_hand_spine_root", "clavicle_RI" },
						{ "clavicle_RI", "upperarm_RI" },
						{ "upperarm_RI", "lowerarm_RI" },
						{ "lowerarm_RI", "hand_RI" }
				};

				FVector2D BoneScreen, PrevBoneScreen, HeadScreen;
				for (const std::pair<const char*, const char*>& Connection : SkeletonConnections)
				{
					auto BONE1 = Connection.first;
					auto BONE2 = Connection.second;

					FVector BoneLoc1 = MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName(BONE1))));
					FVector BoneLoc2 = MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName(BONE2))));

					LocalPlayerController->ProjectWorldLocationToScreen(BoneLoc1, &BoneScreen, false);
					LocalPlayerController->ProjectWorldLocationToScreen(BoneLoc2, &PrevBoneScreen, false);

					if (BoneScreen.IsValid() && PrevBoneScreen.IsValid())
						Draw::DrawLine(BoneScreen.X, BoneScreen.Y, PrevBoneScreen.X, PrevBoneScreen.Y, 1.6f, m_Color);
				}

				float HeadCircleRadius = g_Game.CalculateHeadCircleRadius(Distance);
				FVector Head = MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName("head_equipment"))));
				LocalPlayerController->ProjectWorldLocationToScreen(Head, &HeadScreen, false);

				if (HeadScreen.IsValid())
					Draw::DrawCircleFilled(HeadScreen.X, HeadScreen.Y - 2.5f, HeadCircleRadius, m_Color);
			}

			if (Settings[ESP_SNAP_LINES].Value.bValue)
			{
				Draw::DrawLine(m_ScreenWidth / 2, m_ScreenHeight, (Top.X + Bottom.X) / 2, Bottom.Y, 1.5f, m_Color);
			}

			if (Settings[ESP_DISTANCE].Value.bValue && !Settings[ESP_WEAPON].Value.bValue)
			{
				Draw::DrawString(ImGui::GetIO().FontDefault,
					std::to_string((int)Distance).append(std::string(skCrypt("M"))),
					(Top.X + Bottom.X) / 2, Bottom.Y + 5, 15.f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
			}
			else if (!Settings[ESP_DISTANCE].Value.bValue && Settings[ESP_WEAPON].Value.bValue)
			{
				std::string WeaponName = std::string(skCrypt("----"));
				ABaseMagazineWeapon* EquippedWeapon = Player->GetEquippedWeapon();
				if (UKismetSystemLibrary::IsValid(EquippedWeapon))
				{
					WeaponName = EquippedWeapon->ItemName.ToString();
				}

				Draw::DrawString(ImGui::GetIO().FontDefault,
					WeaponName, (Top.X + Bottom.X) / 2, Bottom.Y + 5, 15.f, true, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (Settings[ESP_DISTANCE].Value.bValue && Settings[ESP_WEAPON].Value.bValue)
			{
				std::string WeaponName = std::string(skCrypt("----"));
				ABaseMagazineWeapon* EquippedWeapon = Player->GetEquippedWeapon();
				if (UKismetSystemLibrary::IsValid(EquippedWeapon))
				{
					WeaponName = EquippedWeapon->ItemName.ToString();
				}

				Draw::DrawString(ImGui::GetIO().FontDefault,
					WeaponName, (Top.X + Bottom.X) / 2, Bottom.Y + 5, 15.f, true, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				Draw::DrawString(ImGui::GetIO().FontDefault,
					std::to_string((int)Distance).append(std::string(skCrypt("M"))),
					(Top.X + Bottom.X) / 2, Bottom.Y + 20, 15.f, true, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			}

			if (Settings[ESP_HEALTH].Value.bValue)
			{
				if (!Player->IsDowned() && !Player->IsArrested() && !Player->IsDeadOrUnconscious())
				{
					float Health = Player->GetCurrentHealth();
					float MaxHealth = Player->GetMaxHealth();
					float Percent = Health * 100.0f / MaxHealth;

					float HWidth = Width / 10;
					if (HWidth < 2.0f)
						HWidth = 2.0f;
					if (HWidth > 3.0f)
						HWidth = 3.0f;

					Draw::VerticalHealthBar(HeadPos.X - (Width / 2.0f) - 5.0f, HeadPos.Y, HWidth, FootPos.Y - HeadPos.Y, (int)Percent, true, BarType::Health);
				}
			}

			if (Settings[ESP_BOX].Value.iValue)
			{
				if (!Player->IsDowned() && !Player->IsArrested() && !Player->IsDeadOrUnconscious())
				{
					switch (Settings[ESP_BOX].Value.iValue)
					{
					case 1:
						Draw::DrawBox(Top.X, Top.Y, Bottom.X, Bottom.Y, true, m_Color);
						break;
					case 2:
						Draw::DrawCornersBox(Top.X, Top.Y, Bottom.X, Bottom.Y, true, true, m_Color);
						break;
					}
				}
			}
		}
	}

	TArray<AEvidenceActor*> Evidences = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllEvidenceActors;
	for (int i = 0; i < Evidences.Count(); i++)
	{
		if (!Settings[ESP_EVIDENCE].Value.bValue)
			break;

		AEvidenceActor* Evidence = Evidences[i];
		if (!UKismetSystemLibrary::IsValid(Evidence))
			continue;

		FVector Location = Evidence->GetActorLocation();
		if (!Location.IsValid())
			continue;

		FVector2D Position;
		LocalPlayerController->ProjectWorldLocationToScreen(Location, &Position, false);

		if (Position.IsValid())
		{
			const float Distance = Evidence->GetDistanceTo(LocalCharacter) / 100.0f;

			Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("[Evidence] ")).append(std::to_string((int)Distance)).append(std::string(skCrypt("M"))),
				Position.X, Position.Y, 15.0f, true, Settings[ESP_EVIDENCE_COLOR].Value.v4Value);
		}
	}

	TArray<ATrapActorAttachedToDoor*> Traps = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllDoorTrapActors;
	for (int i = 0; i < Traps.Count(); i++)
	{
		if (!Settings[ESP_TRAP].Value.bValue)
			break;

		ATrapActorAttachedToDoor* Trap = Traps[i];
		if (!UKismetSystemLibrary::IsValid(Trap))
			continue;

		FVector Location = Trap->GetActorLocation();
		if (!Location.IsValid())
			continue;

		FVector2D Position;
		LocalPlayerController->ProjectWorldLocationToScreen(Location, &Position, false);

		if (Position.IsValid())
		{
			const float Distance = Trap->GetDistanceTo(LocalCharacter) / 100.0f;
			std::string TrapName = Trap->TrapName.IsValid() ? Trap->TrapName.ToString() : "";
			std::string TrapType = g_Game.GetTrapType(Trap->TrapType);

			Draw::DrawString(ImGui::GetIO().FontDefault, TrapType.append(TrapName).append(skCrypt(" ")).append(std::to_string((int)Distance)).append(std::string(skCrypt("M"))),
				Position.X, Position.Y, 15.0f, true, Settings[ESP_TRAP_COLOR].Value.v4Value);
		}
	}
}

void Game::Misc()
{
	UWorld* GWorld = UWorld::GetWorld();
	if (!UKismetSystemLibrary::IsValid(GWorld))
		return;

	if (!UKismetSystemLibrary::IsValid(LocalCharacter))
		return;

	AReadyOrNotPlayerState* LocalPlayerState = static_cast<AReadyOrNotPlayerState*>(LocalCharacter->PlayerState);
	if (!UKismetSystemLibrary::IsValid(LocalPlayerState))
		return;

	if (!LocalPlayerState->bIsInGame)
		return;

	if (Settings[INFINITE_AMMO].Value.bValue)
	{
		ABaseMagazineWeapon* EquippedWeapon = LocalCharacter->GetEquippedWeapon();
		if (UKismetSystemLibrary::IsValid(EquippedWeapon))
		{
			EquippedWeapon->bInfiniteAmmo = ~EquippedWeapon->bInfiniteAmmo;
		}
	}

	if (Settings[NO_RECOIL].Value.bValue)
	{
		ABaseMagazineWeapon* EquippedWeapon = LocalCharacter->GetEquippedWeapon();
		if (UKismetSystemLibrary::IsValid(EquippedWeapon))
		{
			EquippedWeapon->ADSRecoilMultiplier = 0.0f;
			EquippedWeapon->ADSSpreadMultiplier = 0.0f;
			EquippedWeapon->SpreadPattern = FRotator();
			EquippedWeapon->RecoilPattern = TArray<FRotator>();
		}
	}

	if (Settings[BREAK_DOOR].Value.bValue)
	{
		TArray<ADoor*> Doors = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllDoors;
		for (int i = 0; i < Doors.Count(); i++)
		{
			if (UKismetSystemLibrary::IsValid(Doors[i]))
			{
				Doors[i]->BreakDoor(true, LocalCharacter);
			}
		}
	}

	if (Settings[DISARM_TRAP].Value.bValue)
	{
		TArray<ATrapActorAttachedToDoor*> Traps = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllDoorTrapActors;
		for (int i = 0; i < Traps.Count(); i++)
		{
			if (UKismetSystemLibrary::IsValid(Traps[i]))
			{
				Traps[i]->Server_OnTrapDisarmed();
			}
		}
	}

	if (Settings[SPEED].Value.bValue)
	{
		UCharacterMovementComponent* MovementComponent = LocalCharacter->CharacterMovement;
		if (UKismetSystemLibrary::IsValid(MovementComponent))
		{
			MovementComponent->MaxWalkSpeed = Settings[SPEED_MULTIPLIER].Value.iValue;
			MovementComponent->MaxWalkSpeedCrouched = Settings[SPEED_MULTIPLIER].Value.iValue;
			MovementComponent->MaxSwimSpeed = Settings[SPEED_MULTIPLIER].Value.iValue;
			MovementComponent->MaxAcceleration = Settings[SPEED_MULTIPLIER].Value.iValue;
		}
	}

	Settings[NO_CLIP].Value.bValue ? LocalCharacter->SetActorEnableCollision(false) : LocalCharacter->SetActorEnableCollision(true);
	{
		UCharacterMovementComponent* MovementComponent = LocalCharacter->CharacterMovement;
		if (UKismetSystemLibrary::IsValid(MovementComponent))
		{
			std::function<void(bool)> FlyCallback = [MovementComponent](bool bEnableFly)
				{
					MovementComponent->SetMovementMode(bEnableFly ? EMovementMode::MOVE_Flying : EMovementMode::MOVE_Walking, 0);
					MovementComponent->MaxFlySpeed = bEnableFly ? MovementComponent->MaxFlySpeed *= Settings[FLY_MULTIPLIER].Value.iValue : MovementComponent->MaxFlySpeed /= Settings[FLY_MULTIPLIER].Value.iValue;
				};

			FlyCallback(Settings[FLY].Value.bValue);
		}
	}

}

float Game::CalculateHeadCircleRadius(float Distance)
{
	const float MinDistance = 0.0f;
	const float MaxDistance = 100.0f;
	const float MinRadius = 0.5f;
	const float MaxRadius = 6.0f;

	if (Distance > MaxDistance)
	{
		return MinRadius;
	}
	else if (Distance < MinDistance)
	{
		return MaxRadius;
	}
	else
	{
		float t = (Distance - MinDistance) / (MaxDistance - MinDistance);
		return MaxRadius + t * (MinRadius - MaxRadius);
	}
}

std::string Game::GetTrapType(ETrapType Type)
{
	switch (Type)
	{
	case ETrapType::Alarm:
		return std::string(skCrypt("[ALM]"));
	case ETrapType::Flashbang:
		return std::string(skCrypt("[FLB]"));
	case ETrapType::Explosive:
		return std::string(skCrypt("[EXLP]"));
	default:
		return std::string(skCrypt("[UNK]"));
	}
}
