#include <windows.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

// --- 1. AUTOMATIC TOOL INSTALLER ---
// This writes portable terminal tools to the /bin folder on first run
void DeployTerminalTools(const fs::path& binDir) {
    if (!fs::exists(binDir)) fs::create_directory(binDir);

    // Example: Creating a helper tool for the recovery terminal
    fs::path helper = binDir / "recovery_shell.bat";
    if (!fs::exists(helper)) {
        std::ofstream tool(helper);
        tool << "@echo off\necho RecoveryMore Terminal Tools Loaded.\necho Use 'curl', 'diskpart', or 'reagentc' directly.";
        tool.close();
    }
    // You can add more .exe deployments here using raw byte arrays
}

// --- 2. THE CLEANER: Nukes the SD card except for your core files ---
void SanitizeDrive() {
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path currentExe(szPath);
    fs::path currentDir = currentExe.parent_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            std::string name = entry.path().filename().string();

            // PROTECT: The app, the tools, your browser data, and your explorer assets
            if (entry.path() != currentExe && 
                name != "bin" && 
                name != "userdata" && // Chromium profile
                name != "assets" &&   // Explorer UI icons/assets
                name != "update.bat") {
                fs::remove_all(entry.path());
            }
        }
    } catch (...) {}
}

// --- 3. THE UPDATER: Swaps binaries for your GitHub pushes ---
void HandleSelfUpdate() {
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

// --- MAIN ENTRY POINT (WIN32) ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Run the update swap first
    HandleSelfUpdate();

    // Clean the SD card (F:) but protect your hard work
    SanitizeDrive();
    
    // Set up the terminal tools and the PATH
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    fs::path binDir = fs::path(szPath).parent_path() / "bin";
    DeployTerminalTools(binDir);

    // Inject /bin into the current session PATH
    std::wstring envPath = GetEnvironmentVariableW(L"PATH", NULL, 0);
    std::vector<wchar_t> buffer(envPath.length() + binDir.wstring().length() + 2);
    GetEnvironmentVariableW(L"PATH", buffer.data(), (DWORD)envPath.length() + 1);
    std::wstring newPath = binDir.wstring() + L";" + buffer.data();
    SetEnvironmentVariableW(L"PATH", newPath.c_str());

    // --- YOUR CORE UI START ---
    // Start your C++ Browser Engine and File Explorer here.
    
    MessageBoxW(NULL, L"RecoveryMore Started.\nTerminal tools and browser environment ready.", L"RecoveryMore", MB_OK);

    return 0;
}
