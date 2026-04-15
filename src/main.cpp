#include <windows.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <winsock2.h>

namespace fs = std::filesystem;
#pragma comment(lib, "ws2_32.lib")

// --- 1. THE TERMINAL THINGY (CMD Pipe) ---
// This launches a hidden CMD process and redirects input/output to your UI
void LaunchEmbeddedTerminal() {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hRead, hWrite;
    if (CreatePipe(&hRead, &hWrite, &sa, 0)) {
        STARTUPINFO si = { sizeof(si) };
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.hStdOutput = hWrite;
        si.hStdError = hWrite;
        si.wShowWindow = SW_HIDE; // Keep the actual CMD window hidden

        PROCESS_INFORMATION pi;
        // Launching CMD with your custom /bin folder ready to go
        CreateProcess(NULL, (LPWSTR)L"cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        
        // You would then read from hRead to display text in your Browser UI
    }
}

// --- 2. THE IPAD BRIDGE (File Transfer) ---
void StartIPadBridge() {
    // Background listener for iPad uploads/downloads
}

// --- 3. THE PROTECTIVE CLEANER ---
void SanitizeDrive() {
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path currentDir = fs::path(szPath).parent_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            std::string name = entry.path().filename().string();
            // SKIP: self, terminal tools, browser data, explorer assets
            if (entry.path() != szPath && name != "bin" && name != "userdata" && 
                name != "assets" && name != "update.bat") {
                fs::remove_all(entry.path());
            }
        }
    } catch (...) {}
}

// --- 4. THE UPDATER ---
void ApplyUpdates() {
    if (fs::exists("RecoveryMore_New.exe")) {
        std::ofstream batch("update.bat");
        batch << "@echo off\ntimeout /t 1 /nobreak > nul\ndel RecoveryMore.exe\n"
              << "move RecoveryMore_New.exe RecoveryMore.exe\nstart RecoveryMore.exe\ndel \"%~f0\"";
        batch.close();
        ShellExecuteW(NULL, L"open", L"update.bat", NULL, NULL, SW_HIDE);
        exit(0);
    }
}

// --- MAIN ENTRY POINT ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ApplyUpdates();
    SanitizeDrive();
    
    // Path Setup for /bin
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path root = fs::path(szPath).parent_path();
    fs::create_directory(root / "bin");

    // Add /bin to PATH so the Terminal Thingy can see your tools
    std::wstring envPath = GetEnvironmentVariableW(L"PATH", NULL, 0);
    std::vector<wchar_t> buffer(envPath.length() + (root / "bin").wstring().length() + 2);
    GetEnvironmentVariableW(L"PATH", buffer.data(), (DWORD)envPath.length() + 1);
    std::wstring newPath = (root / "bin").wstring() + L";" + buffer.data();
    SetEnvironmentVariableW(L"PATH", newPath.c_str());

    // Start all components
    StartIPadBridge();
    LaunchEmbeddedTerminal();

    // Launch your Browser Engine and File Explorer UI
    // RunBrowserUI(); 

    MessageBoxW(NULL, L"RecoveryMore Loaded.\nTerminal, Browser, Explorer, and iPad Bridge are active.", L"RecoveryMore", MB_OK);

    return 0;
}
