#include <windows.h>
#include <iostream>
#include <fstream>

void AutoInject() {
    // 1. Mount the Recovery Partition silently to 'R:'
    system("echo select disk 0 > s.txt && echo select partition 4 >> s.txt && echo assign letter=R >> s.txt");
    system("diskpart /s s.txt > nul && del s.txt");

    // 2. Create the system folder structure
    CreateDirectory(L"R:\\Sources", NULL);
    CreateDirectory(L"R:\\Sources\\Recovery", NULL);
    CreateDirectory(L"R:\\Sources\\Recovery\\Tools", NULL);

    // 3. Self-Copy: Move this .exe into the Recovery Partition
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    CopyFile(szPath, L"R:\\Sources\\Recovery\\Tools\\RecoveryMore.exe", FALSE);

    // 4. Create the XML so the icon shows up in the blue menu
    std::ofstream xml("R:\\Sources\\Recovery\\Tools\\WinREConfig.xml");
    xml << "<?xml version=\"1.0\" encoding=\"utf-8\"?><Recovery><RecoveryTools>"
        << "<RelativeFilePath>RecoveryMore.exe</RelativeFilePath>"
        << "<CommandLineParam>/autoupdate</CommandLineParam>"
        << "</RecoveryTools></Recovery>";
    xml.close();

    // 5. Register with the system
    system("reagentc /setreimage /path R:\\Recovery\\WindowsRE /target C:\\Windows /customtool R:\\Sources\\Recovery\\Tools\\WinREConfig.xml");
    system("reagentc /enable");
}

int main() {
    // If running from the SD card (D: or E:), auto-inject and exit
    wchar_t drive[4];
    GetModuleFileNameW(NULL, drive, 4);
    if (GetDriveTypeW(drive) == DRIVE_REMOVABLE) {
        AutoInject();
        return 0; 
    }
    
    // Otherwise, just launch your dashboard
    // LaunchRecoveryMoreUI();
    return 0;
}
