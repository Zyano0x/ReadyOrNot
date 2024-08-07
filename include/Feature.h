#pragma once

#define M_PI 3.14159265358979323846 
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI))
#define DEG2RAD(x) ((float)(x) * (float)(M_PI / 180.f))

class Game
{
public:
	explicit Game();
	~Game() noexcept;

	void Hook();
	void UnHook();

	void Setup();
	void Visual();
	void Aimbot();
	void Misc();

	float CalculateHeadCircleRadius(float Distance);
	void RotateTriangle(std::array<FVector, 3>& Points, float Rotation);
	void VectorAnglesRadar(FVector& Forward, FVector& Angles);
	std::string GetTrapType(ETrapType Type);
	FVector2D WorldToRadar(FRotator Rotation, FVector Location, FVector EntityLocation, FVector2D RadarPosition, FVector2D RadarSize);
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
	tGetViewPoint GetViewPoint = nullptr;
	uint64_t GetViewPointAddr = 0;

	typedef void(__fastcall* tServerOnFire)(ABaseMagazineWeapon*, FRotator*, FVector*, int32_t);
	tServerOnFire ServerOnFire = nullptr;
	uint64_t ServerOnFireAddr = 0;

	static void __fastcall ServerOnFireHook(ABaseMagazineWeapon* Weapon, FRotator* Direction, FVector* SpawnLoc, int32_t Seed);
	static void __fastcall GetViewPointHook(ULocalPlayer* LocalPlayer, FMinimalViewInfo* OutViewInfo);
};
inline std::unique_ptr<Game> g_Game = std::make_unique<Game>();