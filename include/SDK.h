#pragma once

#include "UECore.h"

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE]

bool InitSDK(const std::wstring& ModuleName, uintptr_t gObjectsOffset, uintptr_t gNamesOffset, uintptr_t gWorldOffset);
bool InitSDK();

class AActor;
class APlayerState;
class APlayerController;
class AReadyOrNotCharacter;
class UPrimitiveComponent;

//=============================================================================================================================================

enum class ETrapType : uint8_t
{
	Alarm = 0,
	Flashbang = 1,
	Explosive = 2,
	Unknown = 3,
	MAX = 4
};

enum class ETrapState : uint8_t
{
	TS_Live = 0,
	TS_Activated = 1,
	TS_Disabled = 2,
	TS_MAX = 3
};

enum class EMovementMode : uint8_t
{
	MOVE_None = 0,
	MOVE_Walking = 1,
	MOVE_NavWalking = 2,
	MOVE_Falling = 3,
	MOVE_Swimming = 4,
	MOVE_Flying = 5,
	MOVE_Custom = 6,
	MOVE_MAX = 7,
};

enum class ETraceTypeQuery : uint8_t
{
	TraceTypeQuery1 = 0,
	TraceTypeQuery2 = 1,
	TraceTypeQuery3 = 2,
	TraceTypeQuery4 = 3,
	TraceTypeQuery5 = 4,
	TraceTypeQuery6 = 5,
	TraceTypeQuery7 = 6,
	TraceTypeQuery8 = 7,
	TraceTypeQuery9 = 8,
	TraceTypeQuery10 = 9,
	TraceTypeQuery11 = 10,
	TraceTypeQuery12 = 11,
	TraceTypeQuery13 = 12,
	TraceTypeQuery14 = 13,
	TraceTypeQuery15 = 14,
	TraceTypeQuery16 = 15,
	TraceTypeQuery17 = 16,
	TraceTypeQuery18 = 17,
	TraceTypeQuery19 = 18,
	TraceTypeQuery20 = 19,
	TraceTypeQuery21 = 20,
	TraceTypeQuery22 = 21,
	TraceTypeQuery23 = 22,
	TraceTypeQuery24 = 23,
	TraceTypeQuery25 = 24,
	TraceTypeQuery26 = 25,
	TraceTypeQuery27 = 26,
	TraceTypeQuery28 = 27,
	TraceTypeQuery29 = 28,
	TraceTypeQuery30 = 29,
	TraceTypeQuery31 = 30,
	TraceTypeQuery32 = 31,
	TraceTypeQuery_MAX = 32,
	MAX = 33
};

enum class EDrawDebugTrace : uint8_t
{
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	MAX = 4
};

enum class EItemClass : uint8_t
{
	IC_NoClass = 0,
	IC_AssaultRifle = 1,
	IC_SMG = 2,
	IC_LMG = 3,
	IC_Pistol = 4,
	IC_Sniper = 5,
	IC_Melee = 6,
	IC_LessLethal = 7,
	IC_Shotgun = 8,
	IC_Launcher = 9,
	IC_Grenade = 10,
	IC_Shield = 11,
	IC_Armor = 12,
	IC_Headgear = 13,
	IC_TacticalDevice = 14,
	IC_LongTactical = 15,
	IC_Officer = 16,
	IC_Uniform = 17,
	IC_Plates = 18,
	IC_Patches = 19,
	IC_MAX = 20,
};

struct FActorInstanceHandle
{
public:
	TWeakObjectPtr<AActor>                   Actor;                                                   //  ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPrivate
	uint8_t                                  UnknownData_0000[0x10];                                  //  MISSED OFFSET (PADDING)
};

struct FHitResult
{
public:
	int32_t FaceIndex;                                             //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	float Time;                                                    //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	float Distance;                                                //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	uint8_t UnknownData_0000[0x4];                                 //  MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
	FVector Location;                                              //  NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FVector ImpactPoint;                                           //  NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FVector Normal;                                                //  NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FVector ImpactNormal;                                          //  NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FVector TraceStart;                                            //  NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FVector TraceEnd;                                              //  NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	float PenetrationDepth;                                        //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	int32_t MyItem;                                                //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	int32_t Item;                                                  //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	uint8_t ElementIndex;                                          //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	bool bBlockingHit : 1;                                         //  BIT_FIELD NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	bool bStartPenetrating : 1;                                    //  BIT_FIELD NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	uint8_t UnknownData_0001[0x2];                                 //  MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
	uint8_t UnknownData_0002[0x8];								   //  ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FActorInstanceHandle HitObjectHandle;                          //  NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	TWeakObjectPtr<UPrimitiveComponent> Component;                 //  ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FName BoneName;                                                //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FName MyBoneName;                                              //  ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
};

class USceneComponent : public UObject
{
public:
	PAD(0x200);

public:
	FVector GetSocketLocation(class FName InSocketName);
};

class UPrimitiveComponent : public USceneComponent
{
public:
	PAD(0x2D0);
};

class UMeshComponent : public UPrimitiveComponent
{
public:
	PAD(0x40);
};

class USkinnedMeshComponent : public UMeshComponent
{
public:
	PAD(0x340);

public:
	int32_t GetBoneIndex(class FName BoneName);
	FName GetBoneName(int32_t BoneIndex);
	int32_t GetNumBones();
};

class USkeletalMeshComponent : public USkinnedMeshComponent
{
public:
	PAD(0x6B0);

public:
	FVector GetBoneWorldPos(const int32_t& boneId);
};

class UActorComponent : public UObject
{
public:
	PAD(0x78);
};

class UMovementComponent : public UActorComponent
{
public:
	PAD(0x68);
};

class UNavMovementComponent : public UMovementComponent
{
public:
	PAD(0x48);
};

class UPawnMovementComponent : public UNavMovementComponent
{
public:
	PAD(0x8);
};

class UCharacterMovementComponent : public UPawnMovementComponent
{
public:
	PAD(0xA9);
	EMovementMode MovementMode; // 0x201
	PAD(0x46);
	float MaxWalkSpeed; // 0x248
	float MaxWalkSpeedCrouched; // 0x24C
	float MaxSwimSpeed; // 0x250
	float MaxFlySpeed; // 0x254
	PAD(0x4);
	float MaxAcceleration; // 0x25C
	PAD(0xCE8);

public:
	void SetMovementMode(EMovementMode NewMovementMode, uint8_t NewCustomMode);
};

class AActor : public UObject
{
public:
	PAD(0x270);

public:
	void GetActorBounds(bool bOnlyCollidingComponents, struct FVector* Origin, struct FVector* BoxExtent, bool bIncludeFromChildActors);
	FVector GetActorLocation();
	FRotator GetActorRotation();
	USceneComponent* GetRootComponent();
	float GetDistanceTo(AActor* OtherActor);
	void SetActorEnableCollision(bool bNewActorEnableCollision);
};

class APickupActor : public AActor
{
public:
	PAD(0x68);
};

class AEvidenceActor : public APickupActor
{
public:
	PAD(0x58);

public:
	FText GetEvidenceName();
};

class AReportableActor : public AActor
{
public:
	PAD(0xB0);
};

class ATrapActor : public AActor
{
public:
	PAD(0x80);
	FString                                          TrapName;
	ETrapType                                        TrapType;
	ETrapState                                       TrapStatus;
	PAD(0x2E);

public:
	void Server_OnTrapDisarmed();
	void OnTrapDisarmed(AReadyOrNotCharacter* DisarmedBy);
};

class ATrapActorAttachedToDoor : public ATrapActor
{
public:
	PAD(0x98);
};

class ADoorwayWithoutDoor : public AActor
{
public:
	PAD(0x8);
};

class ADoor : public ADoorwayWithoutDoor
{
public:
	PAD(0x1000);

public:
	void BreakDoor(bool bDestroyAllChunks, AReadyOrNotCharacter* DoorBreakerCharacter);
};

class AGameStateBase : public AActor
{
public:
	PAD(0x58);
};

class AReadyOrNotGameState : public AGameStateBase
{
public:
	PAD(0xA0);
	TArray<class AEvidenceActor*> AllEvidenceActors; // 0x390
	TArray<class AReportableActor*> AllReportableActors; // 0x3A0
	TArray<class ATrapActorAttachedToDoor*> AllDoorTrapActors; // 0x3B0
	PAD(0x1B0);
	TArray<class ADoor*> AllDoors; // 0x570
	TArray<class AReadyOrNotCharacter*> AllReadyOrNotCharacters; // 0x580
	TArray<class APlayerCharacter*> AllPlayerCharacters; // 0x590              
	TArray<class ACyberneticCharacter*> AllAICharacters; // 0x5A0
	PAD(0x2D0);
};

class ABaseItem : public AActor
{
public:
	PAD(0x40);
	FText ItemName; // 0x2D8
	PAD(0x40);
	EItemClass ItemClass; // 0x330
	PAD(0x877);
};

class ABaseWeapon : public ABaseItem
{
public:
	PAD(0x468);
	TArray<FRotator> RecoilPattern; // 0x1010
	PAD(0xC);
	float ADSRecoilMultiplier; // 0x102C
	float ADSSpreadMultiplier; // 0x1030
	PAD(0xC);
	FRotator SpreadPattern; // 0x1040
	PAD(0x248);
};

class ABaseMagazineWeapon : public ABaseWeapon
{
public:
	PAD(0x224);
	uint8_t bInfiniteAmmo : 1; // 0x14C4
	PAD(0x25B);
};

class APawn : public AActor
{
public:
	PAD(0x20);
	APlayerState* PlayerState; // 0x2B8
	PAD(0x60);
};

class APlayerCameraManager : public AActor
{
public:
	PAD(0x2128);

public:
	FVector GetCameraLocation();
	FRotator GetCameraRotation();
};

class APlayerState : public AActor
{
public:
	PAD(0xB8);

public:
	APlayerController* GetPlayerController();
	int32_t GetPlayerId();
	FString GetPlayerName();
};

class AReadyOrNotPlayerState : public APlayerState
{
public:
	PAD(0x6C0);
	bool bIsInGame; // 0x0A10
	PAD(0x297);
};

class AController : public AActor
{
public:
	PAD(0x98);

public:
	void SetControlRotation(const FRotator& NewRotation);
	APawn* GetPawn();
	bool LineOfSightTo(AActor* Other, const FVector& ViewPoint, bool bAlternateChecks);
};

class APlayerController : public AController
{
public:
	PAD(0x10);
	APawn* AcknowledgedPawn; // 0x340
	PAD(0x8);
	APlayerCameraManager* PlayerCameraManager; // 0x350
	PAD(0x500);

public:
	bool IsInputKeyDown(const FKey& Key);
	void GetViewportSize(int32_t* SizeX, int32_t* SizeY);
	bool ProjectWorldLocationToScreen(const FVector& WorldLocation, FVector2D* ScreenLocation, bool bPlayerViewportRelative);
};

class ACharacter : public APawn
{
public:
	USkeletalMeshComponent* Mesh; // 0x320
	UCharacterMovementComponent* CharacterMovement; // 0x328
	PAD(0x348);
};

class AReadyOrNotCharacter : public ACharacter
{
public:
	PAD(0x1250);

public:
	bool IsSuspect();
	bool IsCivilian();
	bool IsOnSWATTeam();
	bool IsDowned();
	bool IsArrested();
	bool IsSurrendered();
	bool IsDeadOrUnconscious();
	float GetMaxHealth();
	float GetCurrentHealth();
	ABaseMagazineWeapon* GetEquippedWeapon();
	bool IsOnSameTeam(AReadyOrNotCharacter* B);
	bool IsLocalPlayer();
};

class APlayerCharacter : public AReadyOrNotCharacter
{
public:
	PAD(0x1340);

public:
	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass(std::string(skCrypt("Class /Script/ReadyOrNot.PlayerCharacter")));
		return ptr;
	}
};

class UPlayer : public UObject
{
public:
	PAD(0x8);
	APlayerController* PlayerController; // 0x30
	PAD(0x10);
};

class ULocalPlayer : public UPlayer
{
public:
	PAD(0x268);
};

class UGameInstance : public UObject
{
public:
	PAD(0x10);
	TArray<ULocalPlayer*> LocalPlayers; // 0x38
	PAD(0x178);
};

class ULevel : public UObject
{
public:
	PAD(0x70);
	TArray<AActor*> Actors; // 0x98
	PAD(0x270);

public:
	int32_t GetMaxPacket();
};

class UWorld : public UObject
{
public:
	static inline UWorld** GWorld = nullptr;

public:
	PAD(0x8);
	ULevel* PersistentLevel; // 0x30
	PAD(0x120);
	AGameStateBase* GameState; // 0x150
	PAD(0x10);
	TArray<ULevel*> Levels; // 0x170
	PAD(0x38);
	UGameInstance* OwningGameInstance; // 0x1B8
	PAD(0x728);

public:
	static UWorld* GetWorld();
};

class UKismetSystemLibrary : public UObject
{
public:
	static bool IsValid(UObject* Object);
	static FString GetObjectName(UObject* Object);
	static bool LineTraceSingle(UObject* WorldContextObject, const FVector& Start, const FVector& End, ETraceTypeQuery TraceChannel, EDrawDebugTrace DrawDebugType, FHitResult* OutHit, const FLinearColor& TraceColor, const FLinearColor& TraceHitColor);

public:
	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass(std::string(skCrypt("Class /Script/Engine.KismetSystemLibrary")));
		return ptr;
	}

	static inline UKismetSystemLibrary* GetDefaultObj()
	{
		return GetDefaultObjImpl<UKismetSystemLibrary>();
	}
};

class UKismetMathLibrary : public UObject
{
public:
	static float Vector_Distance(const FVector& v1, const FVector& v2);
	static FRotator FindLookAtRotation(const FVector& Start, const FVector& Target);

public:
	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass(std::string(skCrypt("Class /Script/Engine.KismetMathLibrary")));
		return ptr;
	}

	static inline UKismetMathLibrary* GetDefaultObj()
	{
		return GetDefaultObjImpl<UKismetMathLibrary>();
	}
};