#include "pch.h"

bool InitSDK(const std::wstring& ModuleName, uintptr_t gObjectsOffset, uintptr_t gNamesOffset, uintptr_t gWorldOffset)
{
	auto mBaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleW(ModuleName.c_str()));
	if (!mBaseAddress)
		return false;

	UObject::GObjects = reinterpret_cast<TUObjectArray*>(mBaseAddress + gObjectsOffset);
	FName::GNames = reinterpret_cast<FNamePool*>(mBaseAddress + gNamesOffset);
	UWorld::GWorld = reinterpret_cast<UWorld**>(mBaseAddress + gWorldOffset);

	return true;
}

bool InitSDK()
{
	return InitSDK(std::wstring(MODULE_NAME), OBJECTS_OFFSET, NAMES_OFFSET, WORLD_OFFSET);
}

bool UKismetSystemLibrary::IsValid(UObject* Object)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.KismetSystemLibrary.IsValid")));

	class UKismetSystemLibrary_IsValid_Params
	{
	public:
		UObject* Object;                                              //  0x0000(0x0008)  (ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool ReturnValue;                                             //  0x0008(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	UKismetSystemLibrary_IsValid_Params params{};
	params.Object = Object;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FString UKismetSystemLibrary::GetObjectName(UObject* Object)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.KismetSystemLibrary.GetObjectName")));

	class UKismetSystemLibrary_GetObjectName_Params
	{
	public:
		UObject* Object;                                                  //  0x0000(0x0008)  (ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FString                                          ReturnValue;                                             //  0x0008(0x0010)  (Parm, OutParm, ZeroConstructor, ReturnParm, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	UKismetSystemLibrary_GetObjectName_Params params{};
	params.Object = Object;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool UKismetSystemLibrary::LineTraceSingle(UObject* WorldContextObject, const FVector& Start, const FVector& End, ETraceTypeQuery TraceChannel, EDrawDebugTrace DrawDebugType, FHitResult* OutHit, const FLinearColor& TraceColor, const FLinearColor& TraceHitColor)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.KismetSystemLibrary.LineTraceSingle")));

	class UKismetSystemLibrary_LineTraceSingle_Params
	{
	public:
		UObject* WorldContextObject;                                      //  0x0000(0x0008)  (ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                         Start;                                                   //  0x0008(0x0018)  (ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                         End;                                                     //  0x0020(0x0018)  (ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		ETraceTypeQuery                                      TraceChannel;                                            //  0x0038(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                         bTraceComplex;                                           //  0x0039(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8_t                                                      UnknownData_0034[0x6];                                   //  0x003A(0x0006) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY) ()
		TArray<AActor*>                          ActorsToIgnore;                                          //  0x0040(0x0010)  (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, NativeAccessSpecifierPublic)
		EDrawDebugTrace                                      DrawDebugType;                                           //  0x0050(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8_t                                                      UnknownData_0035[0x7];                                   //  0x0051(0x0007) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY) ()
		FHitResult                                           OutHit;                                                  //  0x0058(0x00E8)  (Parm, OutParm, IsPlainOldData, NoDestructor, ContainsInstancedReference, NativeAccessSpecifierPublic)
		bool                                                         bIgnoreSelf;                                             //  0x0140(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8_t                                                      UnknownData_0036[0x3];                                   //  0x0141(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY) ()
		FLinearColor                                    TraceColor;                                              //  0x0144(0x0010)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FLinearColor                                    TraceHitColor;                                           //  0x0154(0x0010)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                                        DrawTime;                                                //  0x0164(0x0004)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                         ReturnValue;                                             //  0x0168(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	UKismetSystemLibrary_LineTraceSingle_Params params{};
	params.WorldContextObject = WorldContextObject;
	params.Start = Start;
	params.End = End;
	params.TraceChannel = TraceChannel;
	params.bTraceComplex = true;
	params.ActorsToIgnore = TArray<AActor*>();
	params.DrawDebugType = DrawDebugType;
	params.bIgnoreSelf = true;
	params.TraceColor = TraceColor;
	params.TraceHitColor = TraceHitColor;
	params.DrawTime = 0.0f;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	if (OutHit != nullptr)
		*OutHit = params.OutHit;

	return params.ReturnValue;
}

float UKismetMathLibrary::Vector_Distance(const FVector& v1, const FVector& v2)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.KismetMathLibrary.Vector_Distance")));

	class UKismetMathLibrary_Vector_Distance_Params
	{
	public:
		FVector                                         v1;                                                      //  0x0000(0x0018)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                         v2;                                                      //  0x0018(0x0018)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		double                                                       ReturnValue;                                             //  0x0030(0x0008)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	UKismetMathLibrary_Vector_Distance_Params params{};
	params.v1 = v1;
	params.v2 = v2;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FRotator UKismetMathLibrary::FindLookAtRotation(const FVector& Start, const FVector& Target)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.KismetMathLibrary.FindLookAtRotation")));

	class UKismetMathLibrary_FindLookAtRotation_Params
	{
	public:
		FVector                                         Start;                                                   //  0x0000(0x0018)  (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                         Target;                                                  //  0x0018(0x0018)  (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FRotator                                        ReturnValue;                                             //  0x0030(0x0018)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	UKismetMathLibrary_FindLookAtRotation_Params params{};
	params.Start = Start;
	params.Target = Target;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

ECollisionChannel UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery Type)
{
	static uintptr_t ConvertToCollisionChannelAddress = 0;
	if (!ConvertToCollisionChannelAddress)
	{
		ConvertToCollisionChannelAddress = Signature("40 53 48 83 EC ? 8B D9 E8 ? ? ? ? 44 8B C3 B2").GetPointer();
		if (ConvertToCollisionChannelAddress)
			return reinterpret_cast<ECollisionChannel(__fastcall*)(ETraceTypeQuery)>(ConvertToCollisionChannelAddress)(Type);
	}
}

UWorld* UWorld::GetWorld()
{
	if (!GWorld)
		return nullptr;

	return *GWorld;
}

int32_t ULevel::GetMaxPacket()
{
	if (!this)
		return 0;

	return Memory::Read<int32_t>(reinterpret_cast<uintptr_t>(this) + 0xA0);
}

void APlayerCharacter::EquipZipcuffs()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.PlayerCharacter.EquipZipcuffs")));

	class APlayerCharacter_EquipZipcuffs_Params {};

	APlayerCharacter_EquipZipcuffs_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

bool AReadyOrNotCharacter::IsSuspect()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsSuspect")));

	class AReadyOrNotCharacter_IsSuspect_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsSuspect_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsCivilian()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsCivilian")));

	class AReadyOrNotCharacter_IsCivilian_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsCivilian_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsOnSWATTeam()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsOnSWATTeam")));

	class AReadyOrNotCharacter_IsOnSWATTeam_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsOnSWATTeam_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsDowned()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsDowned")));

	class AReadyOrNotCharacter_IsDowned_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsDowned_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsArrested()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsArrested")));

	class AReadyOrNotCharacter_IsArrested_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsArrested_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsSurrendered()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsSurrendered")));

	class AReadyOrNotCharacter_IsSurrendered_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsSurrendered_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsDeadOrUnconscious()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsDeadOrUnconscious")));

	class AReadyOrNotCharacter_IsDeadOrUnconscious_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsDeadOrUnconscious_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsDeadNotUnconscious()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsDeadNotUnconscious")));

	class AReadyOrNotCharacter_IsDeadNotUnconscious_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsDeadNotUnconscious_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsInjured()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsInjured")));

	class AReadyOrNotCharacter_IsInjured_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsInjured_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

float AReadyOrNotCharacter::GetMaxHealth()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.GetMaxHealth")));

	class AReadyOrNotCharacter_GetMaxHealth_Params
	{
	public:
		float                                                        ReturnValue;                                             //  0x0000(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_GetMaxHealth_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

float AReadyOrNotCharacter::GetCurrentHealth()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.GetCurrentHealth")));

	class AReadyOrNotCharacter_GetCurrentHealth_Params
	{
	public:
		float                                                        ReturnValue;                                             //  0x0000(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_GetCurrentHealth_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

ABaseItem* AReadyOrNotCharacter::GetEquippedItem()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.GetEquippedItem")));

	class AReadyOrNotCharacter_GetEquippedItem_Params
	{
	public:
		ABaseItem* ReturnValue;                                             //  0x0000(0x0008)  (Parm, OutParm, ZeroConstructor, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_GetEquippedItem_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

ABaseMagazineWeapon* AReadyOrNotCharacter::GetEquippedWeapon()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.GetEquippedWeapon")));

	class AReadyOrNotCharacter_GetEquippedWeapon_Params
	{
	public:
		ABaseMagazineWeapon* ReturnValue;                                             //  0x0000(0x0008)  (Parm, OutParm, ZeroConstructor, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_GetEquippedWeapon_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void AReadyOrNotCharacter::ArrestComplete(AReadyOrNotCharacter* PlayerMakingArrest, AZipcuffs* Zipcuffs)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.ArrestComplete")));

	class AReadyOrNotCharacter_ArrestComplete_Params
	{
	public:
		AReadyOrNotCharacter* PlayerMakingArrest;                                      //  0x0000(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		AZipcuffs* Zipcuffs;                                                //  0x0008(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_ArrestComplete_Params params{};
	params.PlayerMakingArrest = PlayerMakingArrest;
	params.Zipcuffs = Zipcuffs;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

bool AReadyOrNotCharacter::IsOnSameTeam(AReadyOrNotCharacter* B)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsOnSameTeam")));

	class AReadyOrNotCharacter_IsOnSameTeam_Params
	{
	public:
		AReadyOrNotCharacter* A;                                                       //  0x0000(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		AReadyOrNotCharacter* B;                                                       //  0x0008(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool ReturnValue;                                             //  0x0010(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsOnSameTeam_Params params{};
	params.A = this;
	params.B = B;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AReadyOrNotCharacter::IsLocalPlayer()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.IsLocalPlayer")));

	class AReadyOrNotCharacter_IsLocalPlayer_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_IsLocalPlayer_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void AReadyOrNotCharacter::Server_ReportTarget(AActor* Character)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.Server_ReportTarget")));

	class AReadyOrNotCharacter_Server_ReportTarget_Params
	{
	public:
		AActor* Character;                                               //  0x0000(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_Server_ReportTarget_Params params{};
	params.Character = Character;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

void AReadyOrNotCharacter::Server_CollectEvidence(ABaseItem* Item)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.Server_CollectEvidence")));

	class AReadyOrNotCharacter_Server_CollectEvidence_Params
	{
	public:
		ABaseItem* Item;                                                    //  0x0000(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_Server_CollectEvidence_Params params{};
	params.Item = Item;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

void AReadyOrNotCharacter::Server_CollectEvidenceActor(AEvidenceActor* InEvidenceActor)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.ReadyOrNotCharacter.Server_CollectEvidenceActor")));

	class AReadyOrNotCharacter_Server_CollectEvidenceActor_Params
	{
	public:
		AEvidenceActor* InEvidenceActor;                                         //  0x0000(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AReadyOrNotCharacter_Server_CollectEvidenceActor_Params params{};
	params.InEvidenceActor = InEvidenceActor;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

bool APlayerController::IsInputKeyDown(const FKey& Key)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerController.IsInputKeyDown")));

	class APlayerController_IsInputKeyDown_Params
	{
	public:
		FKey                                              Key;                                                     //  0x0000(0x0018)  (Parm, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                              ReturnValue;                                             //  0x0018(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	APlayerController_IsInputKeyDown_Params params{};
	params.Key = Key;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void APlayerController::GetViewportSize(int32_t* SizeX, int32_t* SizeY)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerController.GetViewportSize")));

	class APlayerController_GetViewportSize_Params
	{
	public:
		int32_t                                                      SizeX;                                                   //  0x0000(0x0004)  (Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32_t                                                      SizeY;                                                   //  0x0004(0x0004)  (Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	APlayerController_GetViewportSize_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	if (SizeX != nullptr)
		*SizeX = params.SizeX;
	if (SizeY != nullptr)
		*SizeY = params.SizeY;
}

bool APlayerController::ProjectWorldLocationToScreen(const FVector& WorldLocation, FVector2D* ScreenLocation, bool bPlayerViewportRelative)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerController.ProjectWorldLocationToScreen")));

	class APlayerController_ProjectWorldLocationToScreen_Params
	{
	public:
		FVector                                         WorldLocation;                                           //  0x0000(0x0018)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector2D                                       ScreenLocation;                                          //  0x0018(0x0010)  (Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                         bPlayerViewportRelative;                                 //  0x0028(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                         ReturnValue;                                             //  0x0029(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	APlayerController_ProjectWorldLocationToScreen_Params params{};
	params.WorldLocation = WorldLocation;
	params.bPlayerViewportRelative = bPlayerViewportRelative;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	if (ScreenLocation != nullptr)
		*ScreenLocation = params.ScreenLocation;

	return params.ReturnValue;
}

APawn* AController::GetPawn()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Controller.K2_GetPawn")));

	class AController_K2_GetPawn_Params
	{
	public:
		APawn* ReturnValue;                                             //  0x0000(0x0008)  (Parm, OutParm, ZeroConstructor, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AController_K2_GetPawn_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

bool AController::LineOfSightTo(AActor* Other, const FVector& ViewPoint, bool bAlternateChecks)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Controller.LineOfSightTo")));

	class AController_LineOfSightTo_Params
	{
	public:
		AActor* Other;                                                   //  0x0000(0x0008)  (ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                         ViewPoint;                                               //  0x0008(0x0018)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                         bAlternateChecks;                                        //  0x0020(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                         ReturnValue;                                             //  0x0021(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AController_LineOfSightTo_Params params{};
	params.Other = Other;
	params.ViewPoint = ViewPoint;
	params.bAlternateChecks = bAlternateChecks;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void AController::SetControlRotation(const FRotator& NewRotation)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Controller.SetControlRotation")));

	class AController_SetControlRotation_Params
	{
	public:
		FRotator                                        NewRotation;                                             //  0x0000(0x0018)  (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	AController_SetControlRotation_Params params{};
	params.NewRotation = NewRotation;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

APlayerController* APlayerState::GetPlayerController()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerState.GetPlayerController")));

	class APlayerState_GetPlayerController_Params
	{
	public:
		APlayerController* ReturnValue;                                             //  0x0000(0x0008)  (Parm, OutParm, ZeroConstructor, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	APlayerState_GetPlayerController_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

int32_t APlayerState::GetPlayerId()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerState.GetPlayerId")));

	class APlayerState_GetPlayerId_Params
	{
	public:
		int32_t                                                      ReturnValue;                                             //  0x0000(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	APlayerState_GetPlayerId_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FString APlayerState::GetPlayerName()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerState.GetPlayerName")));

	class APlayerState_GetPlayerName_Params
	{
	public:
		FString                                          ReturnValue;                                             //  0x0000(0x0010)  (Parm, OutParm, ZeroConstructor, ReturnParm, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	APlayerState_GetPlayerName_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FVector APlayerCameraManager::GetCameraLocation()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerCameraManager.GetCameraLocation")));

	class APlayerCameraManager_GetCameraLocation_Params
	{
	public:
		FVector                                         ReturnValue;                                             //  0x0000(0x0018)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	APlayerCameraManager_GetCameraLocation_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FRotator APlayerCameraManager::GetCameraRotation()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.PlayerCameraManager.GetCameraRotation")));

	class APlayerCameraManager_GetCameraRotation_Params
	{
	public:
		FRotator                                        ReturnValue;                                             //  0x0000(0x0018)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	APlayerCameraManager_GetCameraRotation_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void AZipcuffs::Server_ArrestComplete()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.Zipcuffs.Server_ArrestComplete")));

	class AZipcuffs_Server_ArrestComplete_Params {};

	AZipcuffs_Server_ArrestComplete_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

void ABaseMagazineWeapon::Server_AddMagazine(const FMagazine& Magazine)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.BaseMagazineWeapon.Server_AddMagazine")));

	class ABaseMagazineWeapon_Server_AddMagazine_Params
	{
	public:
		FMagazine                                        Magazine;                                                //  0x0000(0x0004)  (Parm, NoDestructor, NativeAccessSpecifierPublic)
	};

	ABaseMagazineWeapon_Server_AddMagazine_Params params{};
	params.Magazine = Magazine;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

FMagazine ABaseMagazineWeapon::GetCurrentMagazine()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.BaseMagazineWeapon.GetCurrentMagazine")));

	class ABaseMagazineWeapon_GetCurrentMagazine_Params
	{
	public:
		FMagazine                                        ReturnValue;                                             //  0x0000(0x0004)  (Parm, OutParm, ReturnParm, NoDestructor, NativeAccessSpecifierPublic)
	};

	ABaseMagazineWeapon_GetCurrentMagazine_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void ABaseMagazineWeapon::Server_OnFire(const FRotator& Direction, const FVector& SpawnLoc, int32_t Seed)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.BaseMagazineWeapon.Server_OnFire")));

	class ABaseMagazineWeapon_Server_OnFire_Params
	{
	public:
		FRotator                                        Direction;                                               //  0x0000(0x0018)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
		FVector                                         SpawnLoc;                                                //  0x0018(0x0018)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32_t                                         Seed;                                                    //  0x0030(0x0004)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	ABaseMagazineWeapon_Server_OnFire_Params params{};
	params.Direction = Direction;
	params.SpawnLoc = SpawnLoc;
	params.Seed = Seed;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

FText AEvidenceActor::GetEvidenceName()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.EvidenceActor.GetEvidenceName")));

	class AEvidenceActor_GetEvidenceName_Params
	{
	public:
		FText                                            ReturnValue;                                             //  0x0000(0x0018)  (Parm, OutParm, ReturnParm, NativeAccessSpecifierPublic)
	};

	AEvidenceActor_GetEvidenceName_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void ATrapActor::Server_OnTrapDisarmed()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.TrapActor.Server_OnTrapDisarmed")));

	class ATrapActor_Server_OnTrapDisarmed_Params {};

	ATrapActor_Server_OnTrapDisarmed_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

void ATrapActor::OnTrapDisarmed(AReadyOrNotCharacter* DisarmedBy)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.TrapActor.OnTrapDisarmed")));

	class ATrapActor_OnTrapDisarmed_Params
	{
	public:
		AReadyOrNotCharacter* DisarmedBy;                                              //  0x0000(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	ATrapActor_OnTrapDisarmed_Params params{};
	params.DisarmedBy = DisarmedBy;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

void AActor::GetActorBounds(bool bOnlyCollidingComponents, FVector* Origin, FVector* BoxExtent, bool bIncludeFromChildActors)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Actor.GetActorBounds")));

	class AActor_GetActorBounds_Params
	{
	public:
		bool                                                         bOnlyCollidingComponents;                                //  0x0000(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8_t                                                      UnknownData_0019[0x7];                                   //  0x0001(0x0007) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY) ()
		FVector                                         Origin;                                                  //  0x0008(0x0018)  (Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                         BoxExtent;                                               //  0x0020(0x0018)  (Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                                         bIncludeFromChildActors;                                 //  0x0038(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AActor_GetActorBounds_Params params{};
	params.bOnlyCollidingComponents = bOnlyCollidingComponents;
	params.bIncludeFromChildActors = bIncludeFromChildActors;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	if (Origin != nullptr)
		*Origin = params.Origin;
	if (BoxExtent != nullptr)
		*BoxExtent = params.BoxExtent;
}

FVector AActor::GetActorLocation()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Actor.K2_GetActorLocation")));

	class AActor_K2_GetActorLocation_Params
	{
	public:
		FVector                                         ReturnValue;                                             //  0x0000(0x0018)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AActor_K2_GetActorLocation_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FRotator AActor::GetActorRotation()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Actor.K2_GetActorRotation")));

	class AActor_K2_GetActorRotation_Params
	{
	public:
		FRotator                                        ReturnValue;                                             //  0x0000(0x0018)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	AActor_K2_GetActorRotation_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

USceneComponent* AActor::GetRootComponent()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Actor.K2_GetRootComponent")));

	class AActor_K2_GetRootComponent_Params
	{
	public:
		USceneComponent* ReturnValue;                                             //  0x0000(0x0008)  (ExportObject, Parm, OutParm, ZeroConstructor, ReturnParm, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AActor_K2_GetRootComponent_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

float AActor::GetDistanceTo(AActor* OtherActor)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Actor.GetDistanceTo")));

	class AActor_GetDistanceTo_Params
	{
	public:
		AActor* OtherActor;                                              //  0x0000(0x0008)  (ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                                        ReturnValue;                                             //  0x0008(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AActor_GetDistanceTo_Params params{};
	params.OtherActor = OtherActor;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void AActor::SetActorEnableCollision(bool bNewActorEnableCollision)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.Actor.SetActorEnableCollision")));

	class AActor_SetActorEnableCollision_Params
	{
	public:
		bool                                                         bNewActorEnableCollision;                                //  0x0000(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	AActor_SetActorEnableCollision_Params params{};
	params.bNewActorEnableCollision = bNewActorEnableCollision;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

void UBleedComponent::DoHeal()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.BleedComponent.DoHeal")));

	class UBleedComponent_DoHeal_Params {};

	UBleedComponent_DoHeal_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

void UBleedComponent::StopBleeding()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.BleedComponent.StopBleeding")));

	class UBleedComponent_StopBleeding_Params {};

	UBleedComponent_StopBleeding_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

bool UBleedComponent::IsBleeding()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.BleedComponent.IsBleeding")));

	class UBleedComponent_IsBleeding_Params
	{
	public:
		bool                                                         ReturnValue;                                             //  0x0000(0x0001)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	UBleedComponent_IsBleeding_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void UCharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8_t NewCustomMode)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.CharacterMovementComponent.SetMovementMode")));

	class UCharacterMovementComponent_SetMovementMode_Params
	{
	public:
		EMovementMode                                        NewMovementMode;                                         //  0x0000(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8_t                                              NewCustomMode;                                           //  0x0001(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	UCharacterMovementComponent_SetMovementMode_Params params{};
	params.NewMovementMode = NewMovementMode;
	params.NewCustomMode = NewCustomMode;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}

FVector USkeletalMeshComponent::GetBoneWorldPos(const int32_t& boneId)
{
	if (GET_BONE_MATRIX_OFFSET > 0x00)
	{
		typedef void(*GetBoneMatrixType)(const USkinnedMeshComponent*, FMatrix&, int);
		static GetBoneMatrixType GetBoneMatrixPtr = nullptr;
		if (!GetBoneMatrixPtr)
			GetBoneMatrixPtr = reinterpret_cast<GetBoneMatrixType>((uintptr_t)GetModuleHandle(nullptr) + GET_BONE_MATRIX_OFFSET);

		FMatrix matrix;
		GetBoneMatrixPtr(this, matrix, boneId);

		return matrix.WPlane;
	}

	FVector worldPos{};

	/*
	 * How to find the `CachedBoneSpaceTransforms` field:
	 * - Locate the `USkinnedMeshComponent` class in the `Engine` package (Engine_USkinnedMeshComponent.h).
	 * - Find the `MasterPoseComponent` field.
	 * - Find the `PhysicsAssetOverride` field.
	 * - Between these two fields, locate a field with a name starting with `UnknownData_` and of type `uint8_t` with a size of at least 0x20 bytes.
	 * - Within that field, there are two `BasicTypes::TArray<FTransform>` fields; one of them is `CachedBoneSpaceTransforms`.
	 *
	 * How to find the `ComponentToWorld` field:
	 * - Locate the `USceneComponent` class in the `Engine` package (Engine_USceneComponent.h).
	 * - Starting from offset `0x190`, find a field with a name starting with `UnknownData_` and of type `uint8_t` that has at least the size of `FTransform`.
	 *   - This field is typically either the last field in the `USceneComponent` class or the first field in the `UPrimitiveComponent` class (Engine_UPrimitiveComponent.h).
	 * - Search for the `FTransform` where `Scale3D` equals `FVector(1, 1, 1)`.
	 */

	int32_t componentSpaceTransformsOffset = /* 0x400 */ 0x400;
	int32_t componentToWorldOffset = /* 0x190 || 0x1C0 || 0x1D0 || 0x250 */ 0x250;

	uintptr_t thiz = reinterpret_cast<uintptr_t>(this);
	const auto& componentSpaceTransforms = *reinterpret_cast<TArray<FTransform>*>(thiz + componentSpaceTransformsOffset);
	if (boneId >= componentSpaceTransforms.Count())
		return worldPos;

	FMatrix targetBoneMatrix = componentSpaceTransforms[boneId].ToMatrixWithScale();
	FMatrix componentToWorldMatrix = (*reinterpret_cast<FTransform*>(thiz + componentToWorldOffset)).ToMatrixWithScale();
	FMatrix newMatrix = targetBoneMatrix * componentToWorldMatrix;

	worldPos = { newMatrix.WPlane.X, newMatrix.WPlane.Y, newMatrix.WPlane.Z };
	return worldPos;
}

int32_t USkinnedMeshComponent::GetBoneIndex(FName BoneName)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.SkinnedMeshComponent.GetBoneIndex")));

	class USkinnedMeshComponent_GetBoneIndex_Params
	{
	public:
		FName                                            BoneName;                                                //  0x0000(0x0008)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32_t                                                      ReturnValue;                                             //  0x0008(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	USkinnedMeshComponent_GetBoneIndex_Params params{};
	params.BoneName = BoneName;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FName USkinnedMeshComponent::GetBoneName(int32_t BoneIndex)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.SkinnedMeshComponent.GetBoneName")));

	class USkinnedMeshComponent_GetBoneName_Params
	{
	public:
		int32_t                                                      BoneIndex;                                               //  0x0000(0x0004)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FName                                            ReturnValue;                                             //  0x0004(0x0008)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	USkinnedMeshComponent_GetBoneName_Params params{};
	params.BoneIndex = BoneIndex;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

int32_t USkinnedMeshComponent::GetNumBones()
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.SkinnedMeshComponent.GetNumBones")));

	class USkinnedMeshComponent_GetNumBones_Params
	{
	public:
		int32_t                                                      ReturnValue;                                             //  0x0000(0x0004)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	USkinnedMeshComponent_GetNumBones_Params params{};

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

FVector USceneComponent::GetSocketLocation(FName InSocketName)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/Engine.SceneComponent.GetSocketLocation")));

	class USceneComponent_GetSocketLocation_Params
	{
	public:
		FName                                            InSocketName;                                            //  0x0000(0x0008)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector                                         ReturnValue;                                             //  0x0008(0x0018)  (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	USceneComponent_GetSocketLocation_Params params{};
	params.InSocketName = InSocketName;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;

	return params.ReturnValue;
}

void ADoor::BreakDoor(bool bDestroyAllChunks, AReadyOrNotCharacter* DoorBreakerCharacter)
{
	static UFunction* fn = nullptr;
	if (!fn)
		fn = UObject::FindObject<UFunction>(std::string(skCrypt("Function /Script/ReadyOrNot.Door.BreakDoor")));

	class ADoor_BreakDoor_Params
	{
	public:
		bool                                                         bDestroyAllChunks;                                       //  0x0000(0x0001)  (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8_t                                                      UnknownData_0015[0x7];                                   //  0x0001(0x0007) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY) ()
		AReadyOrNotCharacter* DoorBreakerCharacter;                                    //  0x0008(0x0008)  (Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	ADoor_BreakDoor_Params params{};
	params.bDestroyAllChunks = bDestroyAllChunks;
	params.DoorBreakerCharacter = DoorBreakerCharacter;

	auto flags = fn->FunctionFlags;
	UObject::ProcessEvent(fn, &params);
	fn->FunctionFlags = flags;
}