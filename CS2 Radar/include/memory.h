#pragma once
#include <windows.h>
#include <cstdint>

namespace memory {
	extern HWND windowHandle;
	extern HANDLE processHandle;
	extern DWORD processID;

	//attach to our process
	bool attachToProcess();
	//Find the module base of the given process/module
	uintptr_t getModuleBase(const wchar_t* moduleName);

	//Memory Reading functions
	//This version reads directly in to our buffer
	uint64_t memRead(uintptr_t address, void* buffer, size_t size);

	//This version returns the value at the address we are reading
	template<typename T>
	T memRead(uintptr_t address) {
		T value{};
		//If we don't read in the specified number of bytes return empty type object
		if (memRead(address, &value, sizeof(T)) != sizeof(T)) return T{};
		return value;
	}
}
