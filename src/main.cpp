#include <windows.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

// Clears everything in the current folder except the running program
void CleanCurrentDirectory() {
    wchar_t szPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, szPath, MAX_PATH) == 0) return;
    
    fs::path currentExe(szPath);
    fs::path currentDir = currentExe.parent_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            // Never delete the running executable or the update script
            if (entry.path() != currentExe && entry.path().filename() != "update.bat") {
                fs::remove_all(entry.path());
            }
        }
    } catch (...) {
        // Ignore errors if files are locked
    }
}

// Swaps the old exe with the new one if it exists
void ApplyUpdate() {
    if (fs::exists("RecoveryMore_New.exe")) {
        std::ofstream batch("update.bat");
        batch << "@echo off\n"
              << "timeout /t 1 /nobreak > nul\n"
              << "del RecoveryMore.exe\n"
              << "move RecoveryMore_New.exe RecoveryMore.exe\n"
              << "start RecoveryMore.exe\n"
              << "del \"%~f0\"";
        batch.close();

        ShellExecuteW(NULL, L"open", L"update.bat", NULL, NULL, SW_HIDE);
        exit(0);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // 1. Update first so we run the newest version
    ApplyUpdate();

    // 2. Wipe the rest of the SD card/folder to stay clean
    CleanCurrentDirectory();

    // 3. Main Logic
    // This is where your browser tabs and explorer code from src/ would launch
    MessageBoxW(NULL, L"SD Card cleaned. Running latest version.", L"RecoveryMore", MB_OK);

    return 0;
}
