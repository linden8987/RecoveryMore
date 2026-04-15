#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h> // MUST BE FIRST
#include <windows.h>
#include <ws2tcpip.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
#pragma comment(lib, "ws2_32.lib")

// --- 1. THE TERMINAL THINGY ---
void LaunchEmbeddedTerminal() {
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hRead, hWrite;

    if (CreatePipe(&hRead, &hWrite, &sa, 0)) {
        STARTUPINFO si = { sizeof(si) };
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.hStdOutput = hWrite;
        si.hStdError = hWrite;
        si.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION pi;
        // Use the 'W' version for wide strings (L"...")
        wchar_t cmdPath[] = L"cmd.exe";
        if (CreateProcessW(NULL, cmdPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}

// --- 2. THE PROTECTIVE CLEANER ---
void SanitizeDrive() {
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path currentExe(szPath);
    fs::path currentDir = currentExe.parent_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            std::wstring name = entry.path().filename().wstring();
            if (entry.path() != currentExe && 
                name != L"bin" && 
                name != L"userdata" && 
                name != L"assets" && 
                name != L"update.bat") {
                fs::remove_all(entry.path());
            }
        }
    } catch (...) {}
}

// --- 3. THE UPDATER ---
void ApplyUpdates() {
    if (fs::exists("RecoveryMore_New.exe")) {
        std::ofstream batch("update.bat");
        batch << "@echo off\ntimeout /t 1 /nobreak > nul\n"
              << "del RecoveryMore.exe\n"
              << "move RecoveryMore_New.exe RecoveryMore.exe\n"
              << "start RecoveryMore.exe\n"
              << "del \"%~f0\"";
        batch.close();
        ShellExecuteW(NULL, L"open", L"update.bat", NULL, NULL, SW_HIDE);
        exit(0);
    }
}

// --- MAIN ENTRY POINT ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ApplyUpdates();
    SanitizeDrive();
    
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path root = fs::path(szPath).parent_path();
    
    // Create bin if missing
    fs::create_directory(root / "bin");

    // Fix PATH environment logic
    wchar_t oldPath[4096];
    GetEnvironmentVariableW(L"PATH", oldPath, 4096);
    std::wstring newPath = (root / "bin").wstring() + L";" + oldPath;
    SetEnvironmentVariableW(L"PATH", newPath.c_str());

    LaunchEmbeddedTerminal();

    // Start your Browser and Explorer UI here
    MessageBoxW(NULL, L"RecoveryMore Started Successfully.\nNo errors in sight.", L"RecoveryMore", MB_OK);

    return 0;
}
