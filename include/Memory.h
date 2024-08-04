#pragma once

#define MAX_VIRTUALKEYS 0x100

struct VirtualKey
{
	bool bKey, bDown, bUp;
};
inline VirtualKey VirtualKeys[MAX_VIRTUALKEYS];

std::uint8_t* FindSignature(LPCSTR module_name, const std::string& byte_array);
void SwapVTable(void* Obj, void* Func, int Index, void** Ret);
bool GetKeyPress(int VKey, bool Immediate);

class Memory final
{
public:
	class Allocator final
	{
	public:
		~Allocator();

		void Clear();

		void Free(void* data);

		void Free(const void* data);

		void* Allocate(size_t length);

		template <typename T>
		T* Allocate()
		{
			return this->Allocate_Array<T>(1);
		}

		template <typename T>
		T* Allocate_Array(const size_t count = 1)
		{
			auto base = static_cast<T*>(this->Allocate(count * sizeof(T)));

			T();
			return base;
		}

		bool Empty() const;

	private:
		std::mutex mutex_;
		std::vector<void*> pool_;
	};

	static void* Allocate(size_t length);

	template <typename T>
	static inline T* Allocate()
	{
		return Allocate_Array<T>(1);
	}

	template <typename T>
	static inline T* Allocate_Array(const size_t count = 1)
	{
		return static_cast<T*>(Allocate(count * sizeof(T)));
	}

	static void Free(void* data);
	static void Free(const void* data);

	static bool IsSet(const void* mem, char chr, size_t length);

	static Allocator* GetAllocator();

	template <typename DataType>
	inline static DataType Read(uintptr_t Address)
	{
		if (IsBadReadPtr(reinterpret_cast<const void*>(Address), sizeof(DataType)))
			return {};

		return *reinterpret_cast<DataType*>(Address);
	}

	template <typename DataType>
	inline static bool Write(uintptr_t Address, DataType ValueToWrite)
	{
		if (IsBadWritePtr(reinterpret_cast<LPVOID>(Address), sizeof(DataType)))
			return false;

		*reinterpret_cast<DataType*>(Address) = ValueToWrite;
		return true;
	}
private:
	static Allocator mem_allocator_;
};

class Signature
{
public:

	Signature(const std::string& sig);

	Signature Import(const std::string& module_name = std::string((skCrypt("ReadyOrNot-Win64-Shipping.exe"))));

	Signature Add(uint32_t offset);
	Signature Sub(uint32_t offset);
	Signature Instruction(uint32_t offset);

	Signature Dump(const std::string& func_name);

#ifdef _WIN64
	uint64_t GetPointer();
#else
	uint32_t GetPointer();
#endif

private:
#ifdef _WIN64
	uint64_t Pointer;
#else
	uint32_t Pointer;
#endif

	bool Imported;
	std::string Sig;
	std::string Module;
};

class VMTShadow
{
private:
	// Pointer to Target Object
	void* m_Ptr_Object;
	// Pointer to Object Virtual Method Table
	uintptr_t* m_Ptr_Object_VTable;
	// Pointer to Object Fake Virtual Method Table
	uintptr_t* m_Ptr_Object_Fake_VTable;
	// Size of Object Virtual Method Table
	int m_Object_VTable_Size;
	// Map Containing the index hooked and the original function
	std::map<int, uintptr_t*> m_Object_Hooks;

	// Gets the number of functions in a vtable
	int GetVTableSize();

public:
	// Initialize the class for hooking the VMT using the VMT Shadow Hooking technique
	VMTShadow(void* Object);
	~VMTShadow();

	/// <summary>
	/// Apply The Hook on function of VMT
	/// </summary>
	/// <param name="Index">The function index in VMT</param>
	/// <param name="HookFunction">The address of hook function</param>
	/// <returns>Returns the original function pointer</returns>
	uintptr_t* Apply(int Index, uintptr_t* HookFunction);

	/// <summary>
	/// Remove the hook from the hooked function
	/// </summary>
	/// <param name="Index">The function index in VMT</param>
	void Remove(int Index);

	/// <summary>
	/// Free VTable
	/// </summary>
	void FreeFakeVTable();
};