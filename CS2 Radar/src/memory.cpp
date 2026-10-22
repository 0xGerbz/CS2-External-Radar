#include "memory.h"
#include <tlhelp32.h>
namespace memory {
	HWND windowHandle = nullptr;
	HANDLE processHandle = nullptr;
	DWORD processID = 0;

	//attach to our process
	bool attachToProcess() {
		windowHandle = FindWindowW(nullptr, L"CS2.exe");
		if(windowHandle) GetWindowThreadProcessId(windowHandle, &processID);
		processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processID);
		if (processHandle == nullptr) return false;
		return true;
	}

	//Find the module base of the given process/module
	uintptr_t getModuleBase(wchar_t* moduleName) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,GetProcessId(processHandle));
		MODULEENTRY32W entry{};
		entry.dwSize = sizeof(entry);
		uintptr_t base = 0;
		if (Module32FirstW(snapshot, &entry)) {
			do {
				if (lstrcmpiW(entry.szModule, moduleName) == 0) {
					base = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
					break;
				}
			} while (Module32NextW(snapshot, &entry));
		}

		CloseHandle(snapshot);
		return base;
	}

	//Memory Reading functions
	//This version reads directly in to our buffer
	uint64_t memRead(uintptr_t address, void* buffer, size_t size) {
		SIZE_T bytesRead = 0;
		ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead);
		return bytesRead;
	}

	//This version returns the value at the address we are reading
	template<typename T>
	T memRead(uintptr_t address) {
		T value{};
		if (memRead(address, &value, sizeof(T)) != sizeof(T))
			return T{};
		return value;
	}
}