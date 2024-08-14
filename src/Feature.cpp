#include "pch.h"

Game::Game()
{
	InitSDK();

	UKSystemLib = reinterpret_cast<UKismetSystemLibrary*>(UKismetSystemLibrary::StaticClass());
	UKMathLib = reinterpret_cast<UKismetMathLibrary*>(UKismetMathLibrary::StaticClass());
}

Game::~Game() {}

void __fastcall Game::GetViewPointHook(ULocalPlayer* LocalPlayer, FMinimalViewInfo* OutViewInfo)
{
	g_Game->GetViewPoint(LocalPlayer, OutViewInfo);

	if (Settings[FREE_CAM].Value.bValue) {
		static bool GetCameraLocation = true;
		static FVector NewLocation = FVector();

		if (GetCameraLocation) {
			// Set our new location to the original spot once
			NewLocation = OutViewInfo->Location;
			GetCameraLocation = false;
		}

		if (GetAsyncKeyState('W') & 1)
			NewLocation.X += 75.f;
		else if (GetAsyncKeyState('S') & 1)
			NewLocation.X -= 75.f;
		else if (GetAsyncKeyState('A') & 1)
			NewLocation.Y += 75.f;
		else if (GetAsyncKeyState('D') & 1)
			NewLocation.Y -= 75.f;
		else if (GetAsyncKeyState(VK_SPACE) & 1)
			NewLocation.Z += 75.f;
		else if (GetAsyncKeyState(VK_SHIFT) & 1)
			NewLocation.Z -= 75.f;

		// Modify OutViewInfo
		OutViewInfo->Location = NewLocation;
	}

	if (Settings[FOV_CHANGER].Value.bValue) {
		OutViewInfo->FOV = Settings[FOV_AMOUNT].Value.fValue;
	}
	else {
		Settings[FOV_AMOUNT].Value.fValue = OutViewInfo->FOV;
	}
}

void __fastcall Game::ServerOnFireHook(ABaseMagazineWeapon* Weapon, FRotator* Direction, FVector* SpawnLoc, int32_t Seed)
{
	if (Settings[AIM_MODE].Value.iValue == 1) {
		if (g_Game->UKSystemLib->IsValid(g_Game->BestPlayer) && g_Game->UKSystemLib->IsValid(g_Game->LocalPlayerCamera) && Weapon->Owner == g_Game->LocalCharacter) {
			FVector TargetLocation = g_Game->GetAimWorldLocation(g_Game->BestPlayer);
			*SpawnLoc = TargetLocation;
		}
	}

	g_Game->ServerOnFire(Weapon, Direction, SpawnLoc, Seed);
}

void Game::Hook()
{
	GetViewPointAddr = Signature(std::string(skCrypt("48 8B C4 48 89 58 ? 48 89 68 ? 56 57 41 57 48 81 EC ? ? ? ? 0F 29 70"))).GetPointer();
	ServerOnFireAddr = Signature(std::string(skCrypt("40 ? 53 57 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 48 8B ? 45 8B"))).GetPointer();

	if (GetViewPointAddr && ServerOnFireAddr) {
		Hooking::CreateHook(reinterpret_cast<LPVOID>(GetViewPointAddr), &GetViewPointHook, reinterpret_cast<LPVOID*>(&GetViewPoint));
		Hooking::CreateHook(reinterpret_cast<LPVOID>(ServerOnFireAddr), &ServerOnFireHook, reinterpret_cast<LPVOID*>(&ServerOnFire));
	}
}

void Game::UnHook()
{
	Hooking::DisableHook(reinterpret_cast<LPVOID>(GetViewPointAddr));
	Hooking::DisableHook(reinterpret_cast<LPVOID>(ServerOnFireAddr));
}

bool Game::Check()
{
	UWorld* GWorld = UWorld::GetWorld();
	if (!UKSystemLib->IsValid(GWorld))
		return false;

	UGameInstance* OwningGameInstance = GWorld->OwningGameInstance;
	if (!UKSystemLib->IsValid(OwningGameInstance))
		return false;

	ULocalPlayer* LocalPlayer = OwningGameInstance->GetLocalPlayers();
	if (!LocalPlayer)
		return false;

	LocalPlayerController = LocalPlayer->PlayerController;
	if (!UKSystemLib->IsValid(LocalPlayerController))
		return false;

	LocalCharacter = static_cast<APlayerCharacter*>(LocalPlayerController->GetPawn());
	if (!UKSystemLib->IsValid(LocalCharacter))
		return false;

	LocalPlayerCamera = LocalPlayerController->PlayerCameraManager;
	if (!UKSystemLib->IsValid(LocalPlayerCamera))
		return false;

	LocalPlayerController->GetViewportSize(&m_ScreenWidth, &m_ScreenHeight);

	return true;
}

void Game::Visual()
{
	if (!Settings[ESP_ENABLED].Value.bValue)
		return;

	if (!Check())
		return;

	UWorld* GWorld = UWorld::GetWorld();
	if (!UKSystemLib->IsValid(GWorld))
		return;

	AReadyOrNotPlayerState* LocalPlayerState = static_cast<AReadyOrNotPlayerState*>(LocalCharacter->PlayerState);
	if (!UKSystemLib->IsValid(LocalPlayerState))
		return;

	if (!LocalPlayerState->bIsInGame)
		return;

	TArray<AReadyOrNotCharacter*> Players = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllReadyOrNotCharacters;
	for (int i = 0; i < Players.Count(); i++) {
		AReadyOrNotCharacter* Player = Players[i];
		if (!UKSystemLib->IsValid(Player))
			continue;

		USkeletalMeshComponent* MeshComponent = Player->Mesh;
		if (!UKSystemLib->IsValid(MeshComponent))
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

		if (ShowEnemy) {
			if (LocalPlayerController->LineOfSightTo(Player, { 0.0f, 0.0f, 0.0f }, false))
				VEC4CPY(Settings[ESP_VISIBLE_COLOR].Value.v4Value, m_Color);
			else
				VEC4CPY(Settings[ESP_ENEMY_COLOR].Value.v4Value, m_Color);
		}
		else if (ShowFriendly) {
			VEC4CPY(Settings[ESP_FRIENDLY_COLOR].Value.v4Value, m_Color);
		}
		else if (ShowCivilian) {
			VEC4CPY(Settings[ESP_CIVILIAN_COLOR].Value.v4Value, m_Color);
		}

		FVector2D HeadPos, FootPos;
		if (LocalPlayerController->ProjectWorldLocationToScreen({ Location.X, Location.Y, Location.Z - Extend.Z }, &FootPos, false) && LocalPlayerController->ProjectWorldLocationToScreen({ Location.X, Location.Y, Location.Z + Extend.Z }, &HeadPos, false)) {
			const float Distance = Player->GetDistanceTo(LocalCharacter) / 100.0f;
			const float H = FootPos.Y - HeadPos.Y;
			const float W = H / 1.5f;
			const float X = HeadPos.X - W * 0.5f;
			const float Y = HeadPos.Y;

			if (Settings[ESP_NAME].Value.bValue) {
				if (ShowCivilian) {
					Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("[Civilian]")), (X + (HeadPos.X + W * 0.5f)) / 2, Y - 20, 15.0f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
				}
				else if (ShowEnemy) {
					Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("[Suspect]")), (X + (HeadPos.X + W * 0.5f)) / 2, Y - 20, 15.0f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
				}
				else if (ShowFriendly) {
					Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("[Team]")), (X + (HeadPos.X + W * 0.5f)) / 2, Y - 20, 15.0f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
				}
			}

			if (Settings[ESP_SKELETON].Value.bValue) {
				std::vector<std::pair<const char*, const char*>> SkeletonConnections = {
					{ skCrypt("head_equipment"), skCrypt("Head") },
					{ skCrypt("Head"), skCrypt("ik_hand_spine_root") },
					{ skCrypt("ik_hand_spine_root"), skCrypt("torso_stabilizer") },
					{ skCrypt("torso_stabilizer"), skCrypt("ik_hand_spine_root") },
					{ skCrypt("ik_hand_spine_root"), skCrypt("Head") },

					{ skCrypt("torso_stabilizer"), skCrypt("thigh_LE") },
					{ skCrypt("thigh_LE"), skCrypt("calf_LE") },
					{ skCrypt("calf_LE"), skCrypt("foot_LE") },
					{ skCrypt("foot_LE"), skCrypt("ball_LE") },

					{ skCrypt("torso_stabilizer"), skCrypt("thigh_RI") },
					{ skCrypt("thigh_RI"), skCrypt("calf_RI") },
					{ skCrypt("calf_RI"), skCrypt("foot_RI") },
					{ skCrypt("foot_RI"), skCrypt("ball_RI") },

					{ skCrypt("ik_hand_spine_root"), skCrypt("clavicle_LE") },
					{ skCrypt("clavicle_LE"), skCrypt("upperarm_LE") },
					{ skCrypt("upperarm_LE"), skCrypt("lowerarm_LE") },
					{ skCrypt("lowerarm_LE"), skCrypt("hand_LE") },

					{ skCrypt("ik_hand_spine_root"), skCrypt("clavicle_RI") },
					{ skCrypt("clavicle_RI"), skCrypt("upperarm_RI") },
					{ skCrypt("upperarm_RI"), skCrypt("lowerarm_RI") },
					{ skCrypt("lowerarm_RI"), skCrypt("hand_RI") }
				};

				FVector2D BoneScreen, PrevBoneScreen, HeadScreen;
				for (const std::pair<const char*, const char*>& Connection : SkeletonConnections) {
					auto BONE1 = Connection.first;
					auto BONE2 = Connection.second;

					FVector BoneLoc1 = MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName(BONE1))));
					FVector BoneLoc2 = MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName(BONE2))));

					LocalPlayerController->ProjectWorldLocationToScreen(BoneLoc1, &BoneScreen, false);
					LocalPlayerController->ProjectWorldLocationToScreen(BoneLoc2, &PrevBoneScreen, false);

					if (BoneScreen.IsValid() && PrevBoneScreen.IsValid())
						Draw::DrawLine(BoneScreen.X, BoneScreen.Y, PrevBoneScreen.X, PrevBoneScreen.Y, 1.6f, m_Color);
				}

				float HeadCircleRadius = g_Game->CalcHeadCircleRadius(Distance);
				FVector Head = MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName(skCrypt("head_equipment")))));
				LocalPlayerController->ProjectWorldLocationToScreen(Head, &HeadScreen, false);

				if (HeadScreen.IsValid())
					Draw::DrawCircleFilled(HeadScreen.X, HeadScreen.Y - 2.5f, HeadCircleRadius, m_Color);
			}

			if (Settings[ESP_SNAP_LINES].Value.bValue) {
				Draw::DrawLine(m_ScreenWidth / 2, m_ScreenHeight, (X + ((HeadPos.X + W * 0.5f))) / 2, FootPos.Y, 1.5f, m_Color);
			}

			if (Settings[ESP_DISTANCE].Value.bValue && !Settings[ESP_WEAPON].Value.bValue) {
				Draw::DrawString(ImGui::GetIO().FontDefault,
								 std::to_string((int)Distance).append(std::string(skCrypt("M"))),
								 (X + (HeadPos.X + W * 0.5f)) / 2, FootPos.Y + 5, 15.f, true, ImVec4(1.f, 1.f, 1.f, 1.f));
			}
			else if (!Settings[ESP_DISTANCE].Value.bValue && Settings[ESP_WEAPON].Value.bValue) {
				std::string WeaponName = std::string(skCrypt("----"));
				ABaseMagazineWeapon* EquippedWeapon = Player->GetEquippedWeapon();
				if (UKSystemLib->IsValid(EquippedWeapon)) {
					WeaponName = EquippedWeapon->ItemName.ToString();
				}

				Draw::DrawString(ImGui::GetIO().FontDefault,
								 WeaponName, (X + (HeadPos.X + W * 0.5f)) / 2, FootPos.Y + 5, 15.f, true, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (Settings[ESP_DISTANCE].Value.bValue && Settings[ESP_WEAPON].Value.bValue) {
				std::string WeaponName = std::string(skCrypt("----"));
				ABaseMagazineWeapon* EquippedWeapon = Player->GetEquippedWeapon();
				if (UKSystemLib->IsValid(EquippedWeapon)) {
					WeaponName = EquippedWeapon->ItemName.ToString();
				}

				Draw::DrawString(ImGui::GetIO().FontDefault,
								 WeaponName, (X + (HeadPos.X + W * 0.5f)) / 2, FootPos.Y + 5, 15.f, true, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				Draw::DrawString(ImGui::GetIO().FontDefault,
								 std::to_string((int)Distance).append(std::string(skCrypt("M"))),
								 (X + (HeadPos.X + W * 0.5f)) / 2, FootPos.Y + 20, 15.f, true, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			}

			if (Settings[ESP_HEALTH].Value.bValue) {
				if (!Player->IsDowned() && !Player->IsArrested() && !Player->IsDeadOrUnconscious()) {
					float Health = Player->GetCurrentHealth();
					float MaxHealth = Player->GetMaxHealth();
					float Percent = Health * 100.0f / MaxHealth;

					float HWidth = W / 10;
					if (HWidth < 2.0f)
						HWidth = 2.0f;
					if (HWidth > 3.0f)
						HWidth = 3.0f;

					Draw::VerticalHealthBar(HeadPos.X - (W / 2.0f) - 4.0f, HeadPos.Y, HWidth, FootPos.Y - HeadPos.Y, (int)Percent, true, BarType::Health);
				}
			}

			if (Settings[ESP_BOX].Value.iValue) {
				if (!Player->IsDowned() && !Player->IsArrested() && !Player->IsDeadOrUnconscious()) {
					switch (Settings[ESP_BOX].Value.iValue) {
					case 1:
						Draw::DrawBoxOutline(X, Y, W, H, false, m_Color);
						break;
					case 2:
						Draw::DrawBoxOutline(X, Y, W, H, true, m_Color);
						break;
					case 3:
						Draw::DrawCornersBoxOutline(X, Y, W, H, false, m_Color);
						break;
					case 4:
						Draw::DrawCornersBoxOutline(X, Y, W, H, true, m_Color);
						break;
					}
				}
			}
		}
	}

	TArray<ABaseItem*> Items = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllItems;
	for (int i = 0; i < Items.Count(); i++) {
		if (!Settings[ESP_EVIDENCE].Value.bValue)
			break;

		ABaseItem* Item = Items[i];
		if (!UKSystemLib->IsValid(Item))
			continue;

		FVector Location, Extend;
		Item->GetActorBounds(true, &Location, &Extend, false);

		FVector2D HeadPos, FootPos;
		LocalPlayerController->ProjectWorldLocationToScreen({ Location.X, Location.Y, Location.Z - Extend.Z }, &FootPos, false);
		LocalPlayerController->ProjectWorldLocationToScreen({ Location.X, Location.Y, Location.Z + Extend.Z }, &HeadPos, false);

		if (HeadPos.IsValid() && FootPos.IsValid()) {
			const float Distance = Item->GetDistanceTo(LocalCharacter) / 100.0f;
			const float H = FootPos.Y - HeadPos.Y;
			const float W = H / 1.5f;
			const float X = HeadPos.X - W * 0.5f;
			const float Y = HeadPos.Y;

			if (Item->bIsEvidence) {
				Draw::DrawString(ImGui::GetIO().FontDefault, std::string(skCrypt("Evidence ")).append(std::to_string((int)Distance)).append(std::string(skCrypt("M"))),
								 (X + (HeadPos.X + W * 0.5f)) / 2, Y, 15.0f, true, Settings[ESP_EVIDENCE_COLOR].Value.v4Value);
			}
		}
	}

	TArray<ATrapActorAttachedToDoor*> Traps = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllDoorTrapActors;
	for (int i = 0; i < Traps.Count(); i++) {
		if (!Settings[ESP_TRAP].Value.bValue)
			break;

		ATrapActorAttachedToDoor* Trap = Traps[i];
		if (!UKSystemLib->IsValid(Trap))
			continue;

		FVector Location = Trap->GetActorLocation();
		if (!Location.IsValid())
			continue;

		FVector2D Position;
		LocalPlayerController->ProjectWorldLocationToScreen(Location, &Position, false);

		if (Position.IsValid()) {
			const float Distance = Trap->GetDistanceTo(LocalCharacter) / 100.0f;
			std::string TrapName = Trap->TrapName.IsValid() ? Trap->TrapName.ToString() : "";
			std::string TrapType = g_Game->GetTrapType(Trap->TrapType);
			std::string TrapState = g_Game->GetTrapState(Trap->TrapStatus);

			Draw::DrawString(ImGui::GetIO().FontDefault, TrapType.append(TrapState).append(TrapName).append(skCrypt(" ")).append(std::to_string((int)Distance)).append(std::string(skCrypt("M"))),
							 Position.X, Position.Y, 15.0f, true, Settings[ESP_TRAP_COLOR].Value.v4Value);
		}
	}
}

void Game::Aimbot()
{
	if (!Settings[AIM_ENABLED].Value.bValue)
		return;

	if (!Check())
		return;

	if (Settings[FOV].Value.bValue)
		Draw::DrawCircle(m_ScreenWidth / 2, m_ScreenHeight / 2, Settings[FOV_RADIUS].Value.fValue, Settings[FOV_COLOR].Value.v4Value);

	BestPlayer = g_Game->GetBestPlayer();
	if (!UKSystemLib->IsValid(BestPlayer))
		return;

	FVector TargetLocation = g_Game->GetAimWorldLocation(BestPlayer);
	if (!TargetLocation.IsValid())
		return;

	FRotator NewRotation = g_Game->CalcAngle(LocalPlayerCamera->GetCameraLocation(), TargetLocation, LocalPlayerCamera->GetCameraRotation(), Settings[AIM_SMOOTH].Value.fValue);

	if (Settings[AIM_MODE].Value.iValue == 0) {
		if (LocalPlayerController->IsInputKeyDown(FKey(skCrypt("LeftMouseButton")))) {
			LocalPlayerController->SetControlRotation(NewRotation);
		}
	}
}

void Game::Misc()
{
	if (!Check())
		return;

	UWorld* GWorld = UWorld::GetWorld();
	if (!UKSystemLib->IsValid(GWorld))
		return;

	AReadyOrNotPlayerState* LocalPlayerState = static_cast<AReadyOrNotPlayerState*>(LocalCharacter->PlayerState);
	if (!UKSystemLib->IsValid(LocalPlayerState))
		return;

	if (!LocalPlayerState->bIsInGame)
		return;

	if (Settings[INFINITE_AMMO].Value.bValue) {
		ABaseMagazineWeapon* EquippedWeapon = LocalCharacter->GetEquippedWeapon();
		if (UKSystemLib->IsValid(EquippedWeapon)) {
			FMagazine Magazine = EquippedWeapon->GetCurrentMagazine();
			EquippedWeapon->Server_AddMagazine(Magazine);
		}
	}

	if (Settings[NO_RECOIL].Value.bValue) {
		ABaseMagazineWeapon* EquippedWeapon = LocalCharacter->GetEquippedWeapon();
		if (UKSystemLib->IsValid(EquippedWeapon)) {
			EquippedWeapon->ADSRecoilMultiplier = 0.0f;
			EquippedWeapon->ADSSpreadMultiplier = 0.0f;
			EquippedWeapon->SpreadPattern = FRotator();
			EquippedWeapon->PendingSpread = FRotator();
			EquippedWeapon->RecoilPattern = TArray<FRotator>();
		}
	}

	if (Settings[BREAK_DOOR].Value.bValue) {
		TArray<ADoor*> Doors = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllDoors;
		for (int i = 0; i < Doors.Count(); i++) {
			if (UKSystemLib->IsValid(Doors[i]))
				Doors[i]->BreakDoor(true, LocalCharacter);
		}
	}

	if (Settings[DISARM_TRAP].Value.bValue) {
		TArray<ATrapActorAttachedToDoor*> Traps = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllDoorTrapActors;
		for (int i = 0; i < Traps.Count(); i++) {
			if (UKSystemLib->IsValid(Traps[i]))
				Traps[i]->Server_OnTrapDisarmed();
		}
	}

	if (Settings[SPEED].Value.bValue) {
		UCharacterMovementComponent* MovementComponent = LocalCharacter->CharacterMovement;
		if (UKSystemLib->IsValid(MovementComponent)) {
			MovementComponent->MaxWalkSpeed *= Settings[SPEED_MULTIPLIER].Value.iValue;
			MovementComponent->MaxWalkSpeedCrouched *= Settings[SPEED_MULTIPLIER].Value.iValue;
			MovementComponent->MaxSwimSpeed *= Settings[SPEED_MULTIPLIER].Value.iValue;
			MovementComponent->MaxAcceleration *= Settings[SPEED_MULTIPLIER].Value.iValue;
		}
	}

	if (Settings[AUTO_SECURE_EVIDENCE].Value.bValue) {
		TArray<ABaseItem*> Items = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllItems;
		for (int i = 0; i < Items.Count(); i++) {
			ABaseItem* Item = Items[i];
			if (!UKSystemLib->IsValid(Item))
				continue;

			if (Item->bIsEvidence) {
				LocalCharacter->Server_CollectEvidence(Item);
			}
		}
	}

	if (Settings[AUTO_REPORT].Value.bValue || Settings[AUTO_ARREST].Value.bValue) {
		TArray<AReadyOrNotCharacter*> Players = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllReadyOrNotCharacters;
		for (int i = 0; i < Players.Count(); i++) {
			AReadyOrNotCharacter* Player = Players[i];
			if (!UKSystemLib->IsValid(Player))
				continue;

			if (Player->IsLocalPlayer())
				continue;

			if (Settings[AUTO_REPORT].Value.bValue) {
				if (Player->IsDeadNotUnconscious() || Player->IsArrested()) {
					LocalCharacter->Server_ReportTarget(Player);
				}
			}

			if (Settings[AUTO_ARREST].Value.bValue) {
				if (Player->IsCivilian()) {
					Player->ArrestComplete(LocalCharacter, nullptr);
					LocalCharacter->Server_ReportToTOC(Player, false, false);
				}
			}
		}
	}
}

void Game::Radar()
{
	if (!Settings[ESP_RADAR].Value.bValue)
		return;

	if (!Check())
		return;

	UWorld* GWorld = UWorld::GetWorld();
	if (!UKSystemLib->IsValid(GWorld))
		return;

	FVector2D RadarCenter = FVector2D(m_ScreenWidth - Settings[ESP_RADAR_X].Value.fValue, m_ScreenHeight - Settings[ESP_RADAR_Y].Value.fValue);
	TArray<AReadyOrNotCharacter*> Players = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllReadyOrNotCharacters;
	for (int i = 0; i < Players.Count(); i++) {
		AReadyOrNotCharacter* Player = Players[i];
		if (!UKSystemLib->IsValid(Player))
			continue;

		if (Player->IsDeadNotUnconscious())
			continue;

		if (Player->IsLocalPlayer())
			continue;

		bool IsTeammate = Player->IsOnSWATTeam();
		bool IsCivilian = Player->IsCivilian();

		bool ShowEnemy = !IsTeammate && !IsCivilian && Settings[ESP_ENEMY].Value.bValue;
		bool ShowFriendly = IsTeammate && !IsCivilian && Settings[ESP_FRIENDLY].Value.bValue;
		bool ShowCivilian = !IsTeammate && IsCivilian && Settings[ESP_CIVILIAN].Value.bValue;

		if (!(ShowEnemy || ShowFriendly || ShowCivilian))
			continue;

		if (ShowEnemy) {
			if (LocalPlayerController->LineOfSightTo(Player, { 0.0f, 0.0f, 0.0f }, false))
				VEC4CPY(Settings[ESP_VISIBLE_COLOR].Value.v4Value, m_Color);
			else
				VEC4CPY(Settings[ESP_ENEMY_COLOR].Value.v4Value, m_Color);
		}
		else if (ShowFriendly) {
			VEC4CPY(Settings[ESP_FRIENDLY_COLOR].Value.v4Value, m_Color);
		}
		else if (ShowCivilian) {
			VEC4CPY(Settings[ESP_CIVILIAN_COLOR].Value.v4Value, m_Color);
		}

		FVector2D RotatePoint = WorldToRadar(LocalCharacter->GetActorRotation(), LocalCharacter->GetActorLocation(), Player->GetActorLocation(), FVector2D(RadarCenter.X, RadarCenter.Y), 95.0f);

		ImGui::GetForegroundDrawList()->AddLine(ImVec2(RadarCenter.X, RadarCenter.Y), ImVec2(RotatePoint.X, RotatePoint.Y), ImGui::GetColorU32(ImVec4(m_Color.x, m_Color.y, m_Color.z, 0.47f)), 1.0f);
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(RotatePoint.X, RotatePoint.Y), 4.0f, ImGui::GetColorU32(m_Color));
	}
}

float Game::CalcHeadCircleRadius(float Distance)
{
	const float MinDistance = 0.0f;
	const float MaxDistance = 100.0f;
	const float MinRadius = 0.5f;
	const float MaxRadius = 6.0f;

	if (Distance > MaxDistance) {
		return MinRadius;
	}
	else if (Distance < MinDistance) {
		return MaxRadius;
	}
	else {
		float t = (Distance - MinDistance) / (MaxDistance - MinDistance);
		return MaxRadius + t * (MinRadius - MaxRadius);
	}
}

std::string Game::GetTrapType(ETrapType Type)
{
	switch (Type) {
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

std::string Game::GetTrapState(ETrapState State)
{
	switch (State) {
	case ETrapState::TS_Live:
		return std::string(skCrypt("[Live]"));
	case ETrapState::TS_Activated:
		return std::string(skCrypt("[Activated]"));
	case ETrapState::TS_Disabled:
		return std::string(skCrypt("[Disable]"));
	}
}

FVector2D Game::WorldToRadar(FRotator Rotation, FVector Location, FVector EntityLocation, FVector2D RadarCenter, float RadarRadius)
{
	FVector2D DotPos;
	FVector2D Direction;

	// Calculate Direction
	Direction.X = EntityLocation.Y - Location.Y;
	Direction.Y = EntityLocation.X - Location.X;

	// Get Rotation
	float LocalAngles = Rotation.Yaw;
	float Radian = DEG2RAD(LocalAngles);

	// Calculate Raw DotPos
	DotPos.X = Direction.X * cos(Radian) - Direction.Y * sin(Radian);
	DotPos.Y = Direction.X * sin(Radian) + Direction.Y * cos(Radian);

	// Scale DotPos to fit within the radar's radius
	DotPos.X /= 100.0f;
	DotPos.Y /= 100.0f;

	// Adjust DotPos to radar radius
	float Length = sqrt(DotPos.X * DotPos.X + DotPos.Y * DotPos.Y);
	if (Length > RadarRadius) {
		DotPos.X = DotPos.X * RadarRadius / Length;
		DotPos.Y = DotPos.Y * RadarRadius / Length;
	}

	// Convert to radar coordinates (with origin at RadarCenter)
	DotPos.X = DotPos.X + RadarCenter.X;
	DotPos.Y = -DotPos.Y + RadarCenter.Y;

	return DotPos;
}

FVector Game::GetAimWorldLocation(AReadyOrNotCharacter* Player)
{
	USkeletalMeshComponent* MeshComponent = Player->Mesh;
	if (!UKSystemLib->IsValid(MeshComponent))
		return FVector();

	switch (Settings[AIM_BONE].Value.iValue) {
	case 0:
		return MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName("head_equipment"))));
	case 1:
		return MeshComponent->GetSocketLocation(MeshComponent->GetBoneName(MeshComponent->GetBoneIndex(FName("ik_hand_spine_root"))));
	default:
		return FVector();
	}
}

FRotator Game::CalcAngle(FVector Src, FVector Dst, FRotator OldRotation, float Smoothing)
{
	FVector Dir = Dst - Src;
	Dir.GetSafeNormal();
	FRotator Yaptr = Dir.ToRotator();
	FRotator CpYaT = OldRotation;
	Yaptr.Pitch -= CpYaT.Pitch;
	Yaptr.Yaw -= CpYaT.Yaw;
	Yaptr.Roll = 0.f;
	Yaptr.Clamp();
	CpYaT.Pitch += Yaptr.Pitch / Smoothing;
	CpYaT.Yaw += Yaptr.Yaw / Smoothing;
	CpYaT.Roll = 0.f;

	return CpYaT;
}

AReadyOrNotCharacter* Game::GetBestPlayer()
{
	UWorld* GWorld = UWorld::GetWorld();
	if (!UKSystemLib->IsValid(GWorld))
		return nullptr;

	if (!UKSystemLib->IsValid(LocalCharacter))
		return nullptr;

	if (!UKSystemLib->IsValid(LocalPlayerController))
		return nullptr;

	float MinDistance = 133713371337.0f;

	AReadyOrNotCharacter* Out = nullptr;

	TArray<AReadyOrNotCharacter*> Players = static_cast<AReadyOrNotGameState*>(GWorld->GameState)->AllReadyOrNotCharacters;
	TArray<AActor*> Actors = GWorld->PersistentLevel->Actors;
	for (int i = 0; i < Players.Count(); i++) {
		AReadyOrNotCharacter* Player = Players[i];
		if (!UKSystemLib->IsValid(Player))
			continue;

		USkeletalMeshComponent* MeshComponent = Player->Mesh;
		if (!UKSystemLib->IsValid(MeshComponent))
			continue;

		if (Player->IsLocalPlayer())
			continue;

		if (Player->IsDeadNotUnconscious())
			continue;

		if (Player->IsSuspect()) {
			if (Settings[IGNORE_SURRENDERED].Value.bValue && Player->IsSurrendered())
				continue;

			if (Settings[VISIBLE_CHECK].Value.bValue && !LocalPlayerController->LineOfSightTo(Player, { 0.0f, 0.0f, 0.0f }, false))
				continue;

			FVector TargetLocation = GetAimWorldLocation(Player);

			FVector2D Pos;
			LocalPlayerController->ProjectWorldLocationToScreen(TargetLocation, &Pos, false);
			if (Pos.IsValid()) {
				float XC = Pos.X - m_ScreenWidth / 2;
				float YC = Pos.Y - m_ScreenHeight / 2;
				float Distance = sqrtf((XC * XC) + (YC * YC));

				if (Distance <= MinDistance && Distance < Settings[FOV_RADIUS].Value.fValue) {
					MinDistance = Distance;
					Out = Player;
				}
			}
		}
	}
	return Out;
}