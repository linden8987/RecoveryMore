#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <shellapi.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <thread>

namespace fs = std::filesystem;
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shell32.lib")

// --- 1. THE UPDATER: Swaps binaries if a new one is found ---
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

// --- 2. THE CLEANER: Wipes SD card but protects your work ---
void SanitizeDrive() {
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path currentExe(szPath);
    fs::path currentDir = currentExe.parent_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            std::wstring name = entry.path().filename().wstring();
            // SKIP PROTECTED FILES:
            if (entry.path() != currentExe && 
                name != L"bin" && 
                name != L"userdata" && // Browser Data
                name != L"assets" &&   // Explorer UI
                name != L"update.bat") {
                fs::remove_all(entry.path());
            }
        }
    } catch (...) {}
}

// --- 3. THE TERMINAL THINGY: Pipes CMD into your app ---
void LaunchTerminalPipe() {
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hRead, hWrite;

    if (CreatePipe(&hRead, &hWrite, &sa, 0)) {
        STARTUPINFO si = { sizeof(si) };
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.hStdOutput = hWrite;
        si.hStdError = hWrite;
        si.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION pi;
        wchar_t cmd[] = L"cmd.exe";
        if (CreateProcessW(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}

// --- 4. THE IPAD BRIDGE: Miniature Server for file transfers ---
void StartIPadBridge() {
    std::thread serverThread([]() {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        
        sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(8080);

        bind(s, (sockaddr*)&server, sizeof(server));
        listen(s, 3);
        
        // Listener loop would go here to handle iPad connections
    });
    serverThread.detach();
}

// --- MAIN ENTRY POINT ---
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    // Stage 1: Maintenance
    ApplyUpdates();
    SanitizeDrive();

    // Stage 2: Environment Setup
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path root = fs::path(szPath).parent_path();
    fs::create_directory(root / "bin");

    // Add /bin to the session PATH
    wchar_t oldPath[4096];
    GetEnvironmentVariableW(L"PATH", oldPath, 4096);
    std::wstring newPath = (root / "bin").wstring() + L";" + oldPath;
    SetEnvironmentVariableW(L"PATH", newPath.c_str());

    // Stage 3: Launch Components
    StartIPadBridge();
    LaunchTerminalPipe();

    // Launch UI
    MessageBoxW(NULL, L"RecoveryMore Environment Online.\nBrowser and Explorer protected.", L"RecoveryMore", MB_OK);

    return 0;
}
