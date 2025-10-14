#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <string>
#include <objbase.h>

int main(int argc, char* argv[]) {

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };

    std::string arg1 = argv[1];
    std::string arg2 = argv[2];

    std::string args_str = arg1 + " " + arg2;

    LPCSTR args = args_str.c_str();
    sei.lpVerb = "open";
    sei.lpFile = "mouse\\host_cursor.exe";
    sei.lpParameters = args;
    sei.lpDirectory = NULL;
    sei.nShow = SW_SHOWDEFAULT;

    sei.fMask = SEE_MASK_NOCLOSEPROCESS;

    if (ShellExecuteEx(&sei)) {
        std::wcout << L"Successfully opened: " << sei.lpFile << std::endl;
        if (sei.hProcess) {
            std::wcout << L"Process handle obtained." << std::endl;
            CloseHandle(sei.hProcess);
        }
    } else {
        DWORD error = GetLastError();
        std::wcerr << L"ShellExecuteEx failed with error: " << error << std::endl;
    }

    CoUninitialize();
    return 0;
}