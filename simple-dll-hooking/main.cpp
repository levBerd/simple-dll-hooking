#include <iostream>
#include <windows.h>


int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Usage <dll path> <pid>" << std::endl;
		exit(1);
	}

    // check if argv[2] is pid of process name
    char* intcheck;
    strtol(argv[2], &intcheck, 10);
    
    DWORD pid = 0;

    // not a number
    if (*intcheck != 0) {
        std::cout << "[-] Error pid is not a number" << std::endl;
        exit(1);
    }

    pid = std::atoi(argv[2]);
	
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!hProcess)
    {
        std::cerr << "Error opening process, " << GetLastError() << std::endl;
        exit(1);
    }

    auto p = VirtualAllocEx(hProcess, nullptr, 1 << 12, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    WriteProcessMemory(hProcess, p, argv[1], strlen(argv[1]) + 1, nullptr);

    auto hThread = CreateRemoteThread(hProcess, nullptr, 0,
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernelbase.dll"), "LoadLibraryA"),
        p, 0, nullptr);


    CloseHandle(hProcess);


	return 0;
}