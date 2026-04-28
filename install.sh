#!/bin/bash
# RecoveryMore Installer
# You can always update with this.

echo "------------------------------------------"
echo "         RECOVERYMORE INSTALLER           "
echo "------------------------------------------"

# 1. Detection
ARCH=$(uname -m)
echo "System detected: $ARCH"

# 2. Set the Download URL
# This points to the latest ZIP built by your GitHub Actions
REPO_URL="https://github.com/linden8987/RecoveryMore/releases/latest/download/RecoveryMore.zip"
ZIP="https://github.com/ip7z/7zip/releases/download/26.01/7z2601-x64.exe"

# 3. Create a temporary workspace in RAM
mkdir -p /tmp/RecoveryMore
cd /tmp/RecoveryMore

# 4. Download the "Graphical Guy"
echo "Fetching RecoveryMore ZIP from GitHub..."
curl -L $REPO_URL -o RecoveryMore.zip
curl -L $ZIP -o 7zip.exe

if [ $? -eq 0 ]; then
    echo "Download Complete."
    echo "Launching Graphical Interface..."
    
    # 5. Launch 7zip and RecoveryMore
    # In WinRE, we can execute the EXE directly from the shell
    ./7zip.exe
    ./RecoveryMore.zip
else
    echo "Error: Could not reach GitHub. Check your internet connection."
fi
