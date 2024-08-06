#pragma once

class Game
{
public:
	explicit Game();
	~Game() noexcept;

	void Initilize();

	static void Setup();
	static void Visual();
	static void Aimbot();
	static void Misc();

	float CalculateHeadCircleRadius(float Distance);
	std::string GetTrapType(ETrapType Type);
	FVector GetAimWorldLocation(AReadyOrNotCharacter* Player);
	FVector GetDirectionUnitVector(FVector From, FVector To);
	FRotator CalcAngle(FVector Src, FVector Dst, FRotator OldRotation, float Smoothing);
	AReadyOrNotCharacter* GetBestPlayer();
	UGameViewportClient* GetViewport(UWorld* World);

private:
	static inline int m_ScreenWidth = 0;
	static inline int m_ScreenHeight = 0;
	static inline ImVec4 m_Color{};
	static inline AReadyOrNotCharacter* BestPlayer = nullptr;
	static inline APlayerController* LocalPlayerController = nullptr;
	static inline APlayerCharacter* LocalCharacter = nullptr;
	static inline APlayerCameraManager* LocalPlayerCamera = nullptr;
	static inline UKismetSystemLibrary* UKSystemLib = nullptr;
	static inline UKismetMathLibrary* UKMathLib = nullptr;

private:
	typedef void(__fastcall* tGetViewPoint)(ULocalPlayer*, FMinimalViewInfo*);
	tGetViewPoint GetViewPoint = nullptr;

	typedef void(__fastcall* tProcessEvent)(UObject*, UFunction*, void*);
	tProcessEvent ProcessEvent = nullptr;

	typedef void(__fastcall* tServerOnFire)(ABaseMagazineWeapon*, FRotator*, FVector*, int32_t);
	tServerOnFire ServerOnFire = nullptr;

	static void __fastcall ProcessEventHook(UObject* Class, UFunction* Function, void* Parms);
	static void __fastcall ServerOnFireHook(ABaseMagazineWeapon* Weapon, FRotator* Direction, FVector* SpawnLoc, int32_t Seed);
	static void __fastcall GetViewPointHook(ULocalPlayer* LocalPlayer, FMinimalViewInfo* OutViewInfo);
} extern g_Game;