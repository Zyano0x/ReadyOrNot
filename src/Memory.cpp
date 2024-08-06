#include "pch.h"

static void memcpy_(void* _Dst, void const* _Src, size_t _Size)
{
	auto csrc = (char*)_Src;
	auto cdest = (char*)_Dst;

	for (int i = 0; i < _Size; i++)
	{
		cdest[i] = csrc[i];
	}
}

void SwapVTable(void* Obj, void* Func, int Index, void** Ret)
{
	// We get the vtable of viewport client
	auto VTable = *(uintptr_t**)Obj;

	// Now we loop through the vtable. 
	int Methods = 0;
	do {
		++Methods;
	} while (*(uintptr_t*)((uintptr_t)VTable + (Methods * sizeof(uintptr_t))));

	// We make our own copy of the vtable
	auto Fake_VTable = new uint64_t[Methods * sizeof(uintptr_t)];
	for (auto Count = 0; Count < Methods; ++Count)
	{
		Fake_VTable[Count] = *(uintptr_t*)((uintptr_t)VTable + (Count * sizeof(uintptr_t)));

		*Ret = (void*)VTable[Index];

		// Now we switch the function in our own copy and then finally swapping the vtable with ours 
		Fake_VTable[Index] = (uintptr_t)(Func);
		*(uint64_t**)Obj = Fake_VTable;
	}
}

bool GetKeyPress(int VKey, bool Immediate)
{
	if (VirtualKeys[VKey].bKey)
	{
		VirtualKeys[VKey].bUp = false;
		VirtualKeys[VKey].bDown = true;
	}

	else if (!VirtualKeys[VKey].bKey && VirtualKeys[VKey].bDown)
	{
		VirtualKeys[VKey].bUp = true;
		VirtualKeys[VKey].bDown = false;
	}

	else
	{
		VirtualKeys[VKey].bUp = false;
		VirtualKeys[VKey].bDown = false;
	}

	if (Immediate)
		return VirtualKeys[VKey].bDown;

	else
		return VirtualKeys[VKey].bUp;
}

Memory::Allocator Memory::mem_allocator_;

Memory::Allocator::~Allocator()
{
	this->Clear();
}

void Memory::Allocator::Clear()
{
	std::lock_guard _(this->mutex_);

	for (auto& data : this->pool_)
	{
		Memory::Free(data);
	}

	this->pool_.clear();
}

void Memory::Allocator::Free(void* data)
{
	std::lock_guard _(this->mutex_);

	const auto j = std::find(this->pool_.begin(), this->pool_.end(), data);
	if (j != this->pool_.end())
	{
		Memory::Free(data);
		this->pool_.erase(j);
	}
}

void Memory::Allocator::Free(const void* data)
{
	this->Free(const_cast<void*>(data));
}

void* Memory::Allocator::Allocate(const size_t length)
{
	std::lock_guard _(this->mutex_);

	const auto data = Memory::Allocate(length);
	this->pool_.push_back(data);
	return data;
}

bool Memory::Allocator::Empty() const
{
	return this->pool_.empty();
}

void* Memory::Allocate(const size_t length)
{
	return calloc(length, 1);
}

void Memory::Free(void* data)
{
	if (data)
	{
		::free(data);
	}
}

void Memory::Free(const void* data)
{
	free(const_cast<void*>(data));
}

bool Memory::IsSet(const void* mem, const char chr, const size_t length)
{
	const auto mem_arr = static_cast<const char*>(mem);

	for (size_t i = 0; i < length; ++i)
	{
		if (mem_arr[i] != chr)
		{
			return false;
		}
	}

	return true;
}

Memory::Allocator* Memory::GetAllocator()
{
	return &Memory::mem_allocator_;
}

std::uint8_t* FindSignature(LPCSTR module_name, const std::string& byte_array)
{
	HMODULE module = LI_FN(GetModuleHandleA).safe()(module_name);

	if (!module)
		return nullptr;

	static const auto pattern_to_byte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + std::strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;

					if (*current == '?')
						++current;

					bytes.push_back(-1);
				}
				else
				{
					bytes.push_back(std::strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
	const auto nt_headers =
		reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dos_header->e_lfanew);

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte(byte_array.c_str());
	const auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	const auto pattern_size = pattern_bytes.size();
	const auto pattern_data = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - pattern_size; ++i)
	{
		auto found = true;

		for (auto j = 0ul; j < pattern_size; ++j)
		{
			if (scan_bytes[i + j] == pattern_data[j] || pattern_data[j] == -1)
				continue;
			found = false;
			break;
		}
		if (!found)
			continue;
		return &scan_bytes[i];
	}

	return nullptr;
}

Signature::Signature(const std::string& sig)
{
	this->Imported = false;
	this->Sig = sig;
}

Signature Signature::Import(const std::string& module_name)
{
	this->Imported = true;
	this->Module = module_name;
	this->Pointer = (uint64_t)FindSignature(this->Module.data(), this->Sig);
	return *this;
}

Signature Signature::Add(uint32_t value)
{
	if (!this->Imported)
		*this = this->Import();

	this->Pointer += value;
	return *this;
}

Signature Signature::Sub(uint32_t value)
{
	if (!this->Imported)
		*this = this->Import();

	this->Pointer -= value;
	return *this;
}

Signature Signature::Dump(const std::string& func_name)
{
	if (!this->Imported)
		*this = this->Import();

	return *this;
}

Signature Signature::Instruction(uint32_t offset)
{
	if (!this->Imported)
		*this = this->Import();

	this->Pointer = *(int*)(this->Pointer + offset) + this->Pointer;
	return *this;
}

#ifdef _WIN64
uint64_t Signature::GetPointer()
{
	if (!this->Imported)
		*this = this->Import();
	return this->Pointer;
}
#else
uint32_t Signature::GetPointer()
{
	if (!this->imported)
		*this = this->import();
	return this->pointer;
}
#endif

VMTShadow::VMTShadow(void* Object)
{
	// Initialize essential class members
	this->m_Ptr_Object = Object;
	this->m_Ptr_Object_VTable = *reinterpret_cast<uintptr_t**>(Object);
	this->m_Object_VTable_Size = this->GetVTableSize();

	// Creates the Fake VMT
	this->m_Ptr_Object_Fake_VTable = new uintptr_t[this->m_Object_VTable_Size];

	// Fill the Fake Virtual Method Table with the Function Pointers from Original
	memcpy(this->m_Ptr_Object_Fake_VTable, this->m_Ptr_Object_VTable, this->m_Object_VTable_Size * sizeof(uintptr_t));

	// Swap VTable Pointer
	*reinterpret_cast<uintptr_t**>(this->m_Ptr_Object) = this->m_Ptr_Object_Fake_VTable;

	g_Console->cLog(skCrypt("Object: 0x%llX\n"), Console::ECOLOR_GREEN, this->m_Ptr_Object);
	g_Console->cLog(skCrypt("VTable Size: %d\n"), Console::ECOLOR_GREEN, this->m_Object_VTable_Size);
	g_Console->cLog(skCrypt("Original VTable: 0x%llX\n"), Console::ECOLOR_GREEN, this->m_Ptr_Object_VTable);
	g_Console->cLog(skCrypt("Fake VTable: 0x%llX\n"), Console::ECOLOR_GREEN, this->m_Ptr_Object_Fake_VTable);
}

VMTShadow::~VMTShadow()
{
	this->FreeFakeVTable();
}

int VMTShadow::GetVTableSize()
{
	MEMORY_BASIC_INFORMATION mbi{};
	int i = 0;

	// Query memory regions until VirtualQuery fails
	while (VirtualQuery(reinterpret_cast<LPCVOID>(this->m_Ptr_Object_VTable[i]), &mbi, sizeof(mbi)))
	{
#define PAGE_EXECUTABLE ( PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY )

		// Break on invalid pointers
		if ((mbi.State != MEM_COMMIT) || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) || !(mbi.Protect & PAGE_EXECUTABLE))
			break;

		// Increment function count
		++i;
	}

	return i;
}

uintptr_t* VMTShadow::Apply(int Index, uintptr_t* HookFunction)
{
	if (Index < 0 || Index >= m_Object_VTable_Size)
	{
		// Index out of bounds
		return nullptr;
	}

	// Get the Pointer to Original Func
	uintptr_t* p_Original_Function = *reinterpret_cast<uintptr_t**>(this->m_Ptr_Object_Fake_VTable + Index);

	// Swap pointer to Original Function to Hook Function Pointer
	*reinterpret_cast<uintptr_t**>(this->m_Ptr_Object_Fake_VTable + Index) = HookFunction;

	// Insert the hook function to the list
	this->m_Object_Hooks[Index] = p_Original_Function;

	return p_Original_Function;
}

void VMTShadow::Remove(int Index)
{
	// Swap the pointer from hook function to original function using the hook list
	*reinterpret_cast<uintptr_t**>(this->m_Ptr_Object_Fake_VTable + Index) = this->m_Object_Hooks[Index];

	// Removes the hook function from hook list
	this->m_Object_Hooks.erase(Index);
}

void VMTShadow::FreeFakeVTable()
{
	if (m_Ptr_Object_Fake_VTable)
	{
		// Restore the original VTable pointer to the object
		*reinterpret_cast<uintptr_t**>(m_Ptr_Object) = m_Ptr_Object_VTable;

		// Delete the fake VTable
		delete[] m_Ptr_Object_Fake_VTable;
		m_Ptr_Object_Fake_VTable = nullptr;
	}
}