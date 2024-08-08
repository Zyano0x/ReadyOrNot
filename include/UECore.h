#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <unordered_set>

#define FNAME_POOL
#define MODULE_NAME skCrypt(L"ReadyOrNot-Win64-Shipping.exe")
#define OBJECTS_OFFSET 0x8837EB0
#define NAMES_OFFSET 0x8791B00
#define WORLD_OFFSET 0x89A4A58
#define PROCESS_EVENT_INDEX 0x4D
#define PROCESS_EVENT_OFFSET 0xE5FF00
#define CREATE_DEFAULT_OBJECT_INDEX 0x00
#define POST_RENDER_INDEX 0x3C || 0x43
#define GET_BONE_MATRIX_OFFSET 0x00
#define GET_VIEW_POINT_INDEX 0x00
#define GET_PLAYER_VIEW_POINT_OFFSET 0x00

class UObject;

template<typename Fn>
Fn GetVFunction(const void* Instance, const size_t& Index)
{
	auto VTable = *static_cast<const void***>(const_cast<void*>(Instance));
	return reinterpret_cast<Fn>(const_cast<void(*)>(VTable[Index]));
}

//=============================================================================================================================================

template<typename T>
class TArray
{
private:
	T* _data;
	int32_t                                                      _count;
	int32_t                                                      _max;

	friend class FString;

public:
	TArray()
	{
		_data = nullptr;
		_count = 0;
		_max = 0;
	}

	T* Data() const
	{
		return _data;
	}

	const int32_t& Count() const
	{
		return _count;
	}

	const int32_t& Max() const
	{
		return _max;
	}

	const bool& IsValidIndex(int32_t index) const
	{
		return index >= 0 && index < _count;
	}

	T& operator[](const int32_t& index)
	{
		return _data[index];
	}

	const T& operator[](const int32_t& index) const
	{
		return _data[index];
	}
};

class FString : public TArray<wchar_t>
{
public:
	FString();
	FString(const wchar_t* other);
	const wchar_t* wc_str() const;
	const char* c_str() const;
	bool IsValid() const;
	std::string ToString() const;
	std::wstring ToWString() const;
};

//=============================================================================================================================================

enum class EClassCastFlags : uint64_t
{
	None = 0x0000000000000000,

	Field = 0x0000000000000001,
	Int8Property = 0x0000000000000002,
	Enum = 0x0000000000000004,
	Struct = 0x0000000000000008,
	ScriptStruct = 0x0000000000000010,
	Class = 0x0000000000000020,
	ByteProperty = 0x0000000000000040,
	IntProperty = 0x0000000000000080,
	FloatProperty = 0x0000000000000100,
	UInt64Property = 0x0000000000000200,
	ClassProperty = 0x0000000000000400,
	UInt32Property = 0x0000000000000800,
	InterfaceProperty = 0x0000000000001000,
	NameProperty = 0x0000000000002000,
	StrProperty = 0x0000000000004000,
	Property = 0x0000000000008000,
	ObjectProperty = 0x0000000000010000,
	BoolProperty = 0x0000000000020000,
	UInt16Property = 0x0000000000040000,
	Function = 0x0000000000080000,
	StructProperty = 0x0000000000100000,
	ArrayProperty = 0x0000000000200000,
	Int64Property = 0x0000000000400000,
	DelegateProperty = 0x0000000000800000,
	NumericProperty = 0x0000000001000000,
	MulticastDelegateProperty = 0x0000000002000000,
	ObjectPropertyBase = 0x0000000004000000,
	WeakObjectProperty = 0x0000000008000000,
	LazyObjectProperty = 0x0000000010000000,
	SoftObjectProperty = 0x0000000020000000,
	TextProperty = 0x0000000040000000,
	Int16Property = 0x0000000080000000,
	DoubleProperty = 0x0000000100000000,
	SoftClassProperty = 0x0000000200000000,
	Package = 0x0000000400000000,
	Level = 0x0000000800000000,
	Actor = 0x0000001000000000,
	PlayerController = 0x0000002000000000,
	Pawn = 0x0000004000000000,
	SceneComponent = 0x0000008000000000,
	PrimitiveComponent = 0x0000010000000000,
	SkinnedMeshComponent = 0x0000020000000000,
	SkeletalMeshComponent = 0x0000040000000000,
	Blueprint = 0x0000080000000000,
	DelegateFunction = 0x0000100000000000,
	StaticMeshComponent = 0x0000200000000000,
	MapProperty = 0x0000400000000000,
	SetProperty = 0x0000800000000000,
	EnumProperty = 0x0001000000000000,
	USparseDelegateFunction = 0x0002000000000000,
	FMulticastInlineDelegateProperty = 0x0004000000000000,
	FMulticastSparseDelegateProperty = 0x0008000000000000,
	FFieldPathProperty = 0x0010000000000000,
	FLargeWorldCoordinatesRealProperty = 0x0080000000000000,
	FOptionalProperty = 0x0100000000000000,
	FVValueProperty = 0x0200000000000000,
	UVerseVMClass = 0x0400000000000000,
	FVRestValueProperty = 0x0800000000000000,
};

enum class ObjectFlags : uint32_t
{
	None = 0,
	Native = 1 << 25,
	Async = 1 << 26,
	AsyncLoading = 1 << 27,
	Unreachable = 1 << 28,
	PendingKill = 1 << 29,
	RootSet = 1 << 30,
	NoStrongReference = 1 << 31
};

class FUObjectItem
{
public:
	UObject* Object;
	uint8_t Padding_M_Size[0x10];
};

class TUObjectArray
{
private:
	static const constexpr int32_t                               NumElementsPerChunk = 64 * 1024;
	FUObjectItem** Objects;
	FUObjectItem* PreAllocatedObjects;
public:
	int32_t                                                      MaxElements;
	int32_t                                                      NumElements;
	int32_t                                                      MaxChunks;
	int32_t                                                      NumChunks;

public:
	int32_t Count() const;
	int32_t Max() const;
	bool IsValidIndex(int32_t Index) const;
	FUObjectItem* GetObjectPtr(int32_t Index) const;
	UObject* GetByIndex(int32_t index) const;
	FUObjectItem* GetItemByIndex(int32_t index) const;
	UObject* operator[](int32_t i);
	const UObject* operator[](int32_t i) const;
};

class FNameEntryHeader
{
public:
	static const constexpr uint32_t                              ProbeHashBits = 5;
	uint16_t                                                     bIsWide : 1;
	uint16_t                                                     LowercaseProbeHash : ProbeHashBits;
	uint16_t                                                     Len : 10;
};

class FNameEntry
{
public:
	FNameEntryHeader												 Header;
	union
	{
		char                                                         AnsiName[1024];
		wchar_t                                                      WideName[1024];
	};

public:
	int32_t GetLength() const;
	bool IsWide() const;
	int32_t GetId() const;
	std::string GetAnsiName() const;
	std::wstring GetWideName() const;
	std::string GetName() const;
};

class FNameEntryAllocator
{
public:
	static const constexpr int32_t                               Stride = 0x02;
	static const constexpr int32_t                               MaxOffset = Stride * (1 << 16);
	uint8_t                                                      FRWLock[0x8];
	int32_t                                                      CurrentBlock;
	int32_t                                                      CurrentByteCursor;
	uint8_t* Blocks[8192];

public:
	int32_t NumBlocks() const;
	FNameEntry* GetById(int32_t key) const;
	bool IsValidIndex(int32_t key) const;
	bool IsValidIndex(int32_t key, uint32_t block, uint16_t offset) const;
};

class FNamePool
{
public:
	FNameEntryAllocator                              Allocator;
	int32_t                                                      AnsiCount;
	int32_t                                                      WideCount;

public:
	FNameEntry* GetNext(uintptr_t& nextFNameAddress, uint32_t* comparisonId) const;
	int32_t Count() const;
	bool IsValidIndex(int32_t index) const;
	FNameEntry* GetById(int32_t id) const;
	FNameEntry* operator[](int32_t id) const;
};

class FName
{
public:
	static inline FNamePool* GNames = nullptr;
	int32_t                                                      ComparisonIndex;
	int32_t                                                      Number;

public:
	FName();
	FName(int32_t i);
	FName(const char* nameToFind);
	FName(const wchar_t* nameToFind);
	static FNamePool& GetGlobalNames();
	std::string GetNameA() const;
	std::wstring GetNameW() const;
	std::string ToString() const;
	bool operator==(const FName& other) const;
};

class FScriptInterface
{
private:
	UObject* ObjectPointer;
	void* InterfacePointer;

public:
	UObject* GetObjectPtr() const;
	UObject*& GetObjectRef();
	void* GetInterface() const;
};

class FTextData
{
private:
	uint8_t                                                      UnknownData[0x28];
public:
	wchar_t* Name;
	int32_t* Length;
};

class FText
{
private:
	FTextData* Data;
	uint8_t                                                      UnknownData[0x10];

public:
	wchar_t* Get() const;
	std::string ToString() const;
	std::wstring ToWString() const;
};

struct FKey
{
public:
	FName														 KeyName;                                                 //  Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate
	uint8_t                                                      UnknownData_0000[0x10];                                  //  MISSED OFFSET (PADDING)

public:
	inline FKey() : KeyName(), UnknownData_0000{} {}

	inline FKey(const FName InName) : KeyName(InName), UnknownData_0000{} {}

	inline FKey(const char* InName) : KeyName(FName(InName)), UnknownData_0000{} {}

	inline FKey(const TCHAR* InName) : KeyName(FName(InName)), UnknownData_0000{} {}
};

class FWeakObjectPtr
{
public:
	int32_t                                                      ObjectIndex;
	int32_t                                                      ObjectSerialNumber;

public:
	class UObject* Get() const;
	class UObject* operator->() const;
	bool operator==(const FWeakObjectPtr& Other) const;
	bool operator!=(const FWeakObjectPtr& Other) const;
	bool operator==(const class UObject* Other) const;
	bool operator!=(const class UObject* Other) const;
};

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	FWeakObjectPtr												 WeakPtr;
	int32_t                                                      TagAtLastTest;
	TObjectID                                                    ObjectID;
};

class FSoftObjectPath_
{
public:
	FName                                            AssetPathName;
	FString                                          SubPathString;
};

class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath_>
{
public:
	FName GetAssetPathName() const;
	FString GetSubPathString() const;
	std::string GetAssetPathNameStr() const;
	std::string GetSubPathStringStr() const;
};

template<typename T, typename TWeakObjectPtrBase = FWeakObjectPtr>
class TWeakObjectPtr : public TWeakObjectPtrBase
{
public:
	T* Get() const
	{
		return (T*)TWeakObjectPtrBase::Get();
	}

	T& operator*() const
	{
		return *Get();
	}

	T* operator->() const
	{
		return Get();
	}

	bool IsValid()
	{
		return TWeakObjectPtrBase::IsValid();
	}
};

//=============================================================================================================================================

class UObject
{
public:
	static inline TUObjectArray* GObjects = nullptr;               //  PREDEFINED PROPERTY
	void* VfTable;                                                 //  PREDEFINED PROPERTY
	int32_t Flags;                                                 //  PREDEFINED PROPERTY
	int32_t InternalIndex;                                         //  PREDEFINED PROPERTY
	class UClass* Class;                                           //  PREDEFINED PROPERTY
	FName Name;                                                    //  PREDEFINED PROPERTY
	UObject* Outer;                                                //  PREDEFINED PROPERTY

public:
	void ExecuteUbergraph(int32_t EntryPoint);
	void ProcessEvent(class UFunction* function, void* params);
	static TUObjectArray& GetGlobalObjects();
	std::string GetName() const;
	std::string GetFullName() const;

	template<typename T>
	static T* FindObject(const std::string& name)
	{
		auto& gObjects = UObject::GetGlobalObjects();
		for (int32_t i = 0; i < gObjects.Count(); ++i)
		{
			auto object = gObjects.GetByIndex(i);

			if (!object)
				continue;

			if (object->GetFullName() == name)
				return static_cast<T*>(object);
		}
		return nullptr;
	}

	template<typename T>
	static T* FindObject()
	{
		auto& gObjects = UObject::GetGlobalObjects();
		auto v = T::StaticClass();
		for (int32_t i = 0; i < gObjects.Count(); ++i)
		{
			auto object = gObjects.GetByIndex(i);

			if (!object)
				continue;

			if (object->IsA(v))
				return static_cast<T*>(object);
		}
		return nullptr;
	}

	template<typename T>
	static std::vector<T*> FindObjects(const std::string& name)
	{
		auto& gObjects = UObject::GetGlobalObjects();
		std::vector<T*> ret;
		for (int32_t i = 0; i < gObjects.Count(); ++i)
		{
			auto object = gObjects.GetByIndex(i);

			if (!object)
				continue;

			if (object->GetFullName() == name)
				ret.push_back(static_cast<T*>(object));
		}
		return ret;
	}

	template<typename T>
	static std::vector<T*> FindObjects()
	{
		std::vector<T*> ret;
		auto v = T::StaticClass();
		auto& gObjects = UObject::GetGlobalObjects();
		for (int i = 0; i < gObjects.Count(); ++i)
		{
			auto object = gObjects.GetByIndex(i);

			if (!object)
				continue;

			if (object->IsA(v))
				ret.push_back(static_cast<T*>(object));
		}
		return ret;
	}

	static UClass* FindClass(const std::string& name);

	template<typename T>
	static T* GetObjectCasted(size_t index)
	{
		return static_cast<T*>(UObject::GetGlobalObjects().GetByIndex(index));
	}

	bool IsA(UClass* cmp) const;

	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass("Class /Script/CoreUObject.Object");
		return ptr;
	}
};

class UField : public UObject
{
public:
	UField* Next;                                                    //  PREDEFINED PROPERTY

public:
	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass("Class /Script/CoreUObject.Field");
		return ptr;
	}
};

class UStruct : public UField
{
public:
	uint8_t Padding_M_FrontSize[0x10];                               //  PREDEFINED PROPERTY
	UStruct* SuperField;                                             //  PREDEFINED PROPERTY
	UField* Children;                                                //  PREDEFINED PROPERTY
	struct FPointer* ChildProperties;                                //  PREDEFINED PROPERTY
	int32_t PropertySize;                                            //  PREDEFINED PROPERTY
	int32_t MinAlignment;                                            //  PREDEFINED PROPERTY

public:
	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass("Class /Script/CoreUObject.Struct");
		return ptr;
	}
};

class UClass : public UStruct
{
public:
	uint8_t UnknownData_0000[0x28];
	EClassCastFlags CastFlags;
	uint8_t UnknownData_0001[0x30];
	UObject* DefaultObject;
	uint8_t UnknownData_0002[0xE8];

public:
	UObject* CreateDefaultObject();

	template<typename T>
	T* CreateDefaultObjectOfType()
	{
		return static_cast<T*>(CreateDefaultObject());
	}

	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass("Class /Script/CoreUObject.Class");
		return ptr;
	}
};

class UFunction : public UStruct
{
public:
	uint8_t                                                      Padding_M_FrontSize[0x50];                               //  PREDEFINED PROPERTY
	int32_t                                                      FunctionFlags;                                           //  PREDEFINED PROPERTY
	uint8_t                                                      Padding_M_1[0x24];                                       //  PREDEFINED PROPERTY
	void* Func;                                                    //  PREDEFINED PROPERTY

public:
	static inline UClass* StaticClass()
	{
		static UClass* ptr = nullptr;
		if (!ptr)
			ptr = UObject::FindClass("Class /Script/CoreUObject.Function");
		return ptr;
	}
};

//==============================================================================================================================================

class FFieldVariant
{
public:
	using ContainerType = union { class FField* Field; class UObject* Object; };             // 0x0000(0x0008)(NOT AUTO-GENERATED PROPERTY)

	static constexpr uint64_t UObjectMask = 0x1;                                 // 0x0000(0x0001)(NOT AUTO-GENERATED PROPERTY)

	ContainerType Container;                                         // 0x0000(0x0008)(NOT AUTO-GENERATED PROPERTY)
};

class FField
{
public:
	void* VTable;                                            // 0x0000(0x0008)(NOT AUTO-GENERATED PROPERTY)
	class FFieldClass* ClassPrivate;                         // 0x0008(0x0008)(NOT AUTO-GENERATED PROPERTY)
	FFieldVariant Owner;                                     // 0x0010(0x0008)(NOT AUTO-GENERATED PROPERTY)
	class FField* Next;                                      // 0x0018(0x0008)(NOT AUTO-GENERATED PROPERTY)
	FName Name;                                              // 0x0020(0x0008)(NOT AUTO-GENERATED PROPERTY)
	int32_t ObjFlags;                                        // 0x0028(0x0004)(NOT AUTO-GENERATED PROPERTY)
};

class FProperty : public FField
{
public:
	int32_t                                         ArrayDim;                                          // 0x0030(0x0004)(NOT AUTO-GENERATED PROPERTY)
	int32_t                                         ElementSize;                                       // 0x0034(0x0004)(NOT AUTO-GENERATED PROPERTY)
	uint64_t                                        PropertyFlags;                                     // 0x0038(0x0008)(NOT AUTO-GENERATED PROPERTY)
	uint8_t                                         Pad_40[0x4];                                       // 0x0040(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	int32_t                                         Offset;                                            // 0x0044(0x0004)(NOT AUTO-GENERATED PROPERTY)
	uint8_t                                         Pad_48[0x28];                                      // 0x0048(0x0028)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class FOutputDevice
{
	void* VTable;
	bool bSuppressEventTag;
	bool bAutoEmitLineTerminator;
};

//==============================================================================================================================================

struct FRotator
{
public:
	double                                                       Pitch;                                                   //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       Yaw;                                                     //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       Roll;                                                    //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic

public:
	FRotator();
	FRotator(double pitch, double yaw, double roll);
	FRotator operator +(double scalar) const;
	FRotator operator +(const FRotator& other) const;
	FRotator operator -(double scalar) const;
	FRotator operator -(const FRotator& other) const;
	FRotator operator *(double scalar) const;
	FRotator operator *(const FRotator& other) const;
	FRotator operator /(double scalar) const;
	FRotator operator /(const FRotator& other) const;
	FRotator& operator =(const FRotator& other);
	FRotator& operator +=(double scalar);
	FRotator& operator +=(const FRotator& other);
	FRotator& operator -=(double scalar);
	FRotator& operator -=(const FRotator& other);
	FRotator& operator *=(const FRotator& other);
	FRotator& operator *=(const double other);
	FRotator& operator /=(const FRotator& other);
	FRotator& operator /=(const double other);
	double Size() const;
	FRotator Clamp() const;
};

struct FVector
{
public:
	double                                                       X;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       Y;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       Z;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic

public:
	FVector();
	FVector(double x, double y, double z);
	FVector operator +(double scalar) const;
	FVector operator +(const FVector& other) const;
	FVector operator -(double scalar) const;
	FVector operator -(const FVector& other) const;
	FVector operator *(double scalar) const;
	FVector operator *(const FVector& other) const;
	FVector operator /(double scalar) const;
	FVector operator /(const FVector& other) const;
	FVector& operator =(const FVector& other);
	FVector& operator +=(double scalar);
	FVector& operator +=(const FVector& other);
	FVector& operator -=(double scalar);
	FVector& operator -=(const FVector& other);
	FVector& operator *=(const FVector& other);
	FVector& operator *=(double scalar);
	bool IsValid();
	void Normalize360(FVector& angle);
	double Dot(const FVector& b) const;
	double MagnitudeSqr() const;
	double Magnitude() const;
	double InvSqrt(float F) const;
	FVector Unit() const;
	FRotator ToRotator() const;
	FVector GetSafeNormal(float Tolerance = 0.0f) const;
	double Distance(const FVector& v) const;
	double DistanceMeter(FVector& v) const;

	friend bool operator ==(const FVector& first, const FVector& second)
	{
		return first.X == second.X && first.Y == second.Y && first.Z == second.Z;
	}

	friend bool operator !=(const FVector& first, const FVector& second)
	{
		return !(first == second);
	}
};

struct FVector2D
{
public:
	double                                                       X;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       Y;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic

public:
	FVector2D();
	FVector2D(double x, double y);
	bool IsValid();
	double Distance(FVector2D& v) const;
	double Dot(const FVector2D& b) const;
	double MagnitudeSqr() const;
	double Magnitude() const;
	FVector2D Normalize() const;
	FVector2D operator +(const FVector2D& other) const;
	FVector2D operator +(double scalar) const;
	FVector2D operator -(double scalar) const;
	FVector2D operator -(const FVector2D& other) const;
	FVector2D operator *(double scalar) const;
	FVector2D operator *(const FVector2D& other) const;
	FVector2D operator /(double scalar) const;
	FVector2D operator /(const FVector2D& other) const;
	FVector2D& operator =(const FVector2D& other);
	FVector2D& operator +=(double scalar);
	FVector2D& operator +=(const FVector2D& other);
	FVector2D& operator -=(double scalar);
	FVector2D& operator -=(const FVector2D& other);
	FVector2D& operator *=(const FVector2D& other);
	FVector2D& operator *=(double scalar);
};

struct FLinearColor
{
public:
	float                                                        R;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	float                                                        G;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	float                                                        B;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	float                                                        A;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic

public:
	FLinearColor();
	FLinearColor(float r, float g, float b, float a);
};

struct FPlane : public FVector
{
public:
	double                                                       W;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
};

struct FMatrix
{
public:
	FPlane                                          XPlane;                                                  //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FPlane                                          YPlane;                                                  //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FPlane                                          ZPlane;                                                  //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FPlane                                          WPlane;                                                  //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic

public:
	FMatrix operator *(const FMatrix& other) const;
};

struct FQuat
{
public:
	double                                                       X;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       Y;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       Z;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	double                                                       W;                                                       //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
};

struct FTransform
{
public:
	FQuat                                           Rotation;                                                //  Edit, BlueprintVisible, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	FVector                                         Translation;                                             //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	uint8_t                                                      UnknownData_0000[0x8];                                   //  MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
	FVector                                         Scale3D;                                                 //  Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
	uint8_t                                                      UnknownData_0001[0x8];                                   //  MISSED OFFSET (PADDING)

public:
	FMatrix ToMatrixWithScale() const;
};