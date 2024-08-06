#pragma once

class Game
{
public:
	explicit Game();
	~Game() noexcept;

	void Initilize();
	void UnHook();

	void Setup();
	void Visual();
	void Aimbot();
	void Misc();

	float CalculateHeadCircleRadius(float Distance);
	std::string GetTrapType(ETrapType Type);
	FVector GetAimWorldLocation(AReadyOrNotCharacter* Player);
	FVector GetDirectionUnitVector(FVector From, FVector To);
	FRotator CalcAngle(FVector Src, FVector Dst, FRotator OldRotation, float Smoothing);
	AReadyOrNotCharacter* GetBestPlayer();
	UGameViewportClient* GetViewport(UWorld* World);

private:
	int m_ScreenWidth = 0;
	int m_ScreenHeight = 0;
	ImVec4 m_Color{};
	AReadyOrNotCharacter* BestPlayer = nullptr;
	APlayerController* LocalPlayerController = nullptr;
	APlayerCharacter* LocalCharacter = nullptr;
	APlayerCameraManager* LocalPlayerCamera = nullptr;
	UKismetSystemLibrary* UKSystemLib = nullptr;
	UKismetMathLibrary* UKMathLib = nullptr;

private:
	typedef void(__fastcall* tGetViewPoint)(ULocalPlayer*, FMinimalViewInfo*);
	uint64_t GetViewPoint = 0;
	std::unique_ptr<PLH::x64Detour> GetViewPointDetour = nullptr;

	typedef void(__fastcall* tServerOnFire)(ABaseMagazineWeapon*, FRotator*, FVector*, int32_t);
	uint64_t ServerOnFire = 0;
	std::unique_ptr<PLH::x64Detour> ServerOnFireDetour = nullptr;

	static void __fastcall ServerOnFireHook(ABaseMagazineWeapon* Weapon, FRotator* Direction, FVector* SpawnLoc, int32_t Seed);
	static void __fastcall GetViewPointHook(ULocalPlayer* LocalPlayer, FMinimalViewInfo* OutViewInfo);
};
inline std::unique_ptr<Game> g_Game;