#!/bin/bash
# RecoveryMore Automated GUI Installer
# Architect: Linden | Mode: Cloud-Streamed GUI

echo "------------------------------------------"
echo "   RECOVERYMORE: GRAPHICAL INSTALLER    "
echo "------------------------------------------"

# 1. Detection
ARCH=$(uname -m)
echo "System detected: $ARCH"

# 2. Set the Download URL
# This points to the latest EXE built by your GitHub Actions
REPO_URL="https://github.com/linden8987/RecoveryMore/releases/latest/download/RecoveryMore.zip"

# 3. Create a temporary workspace in RAM
mkdir -p /tmp/RecoveryMore
cd /tmp/RecoveryMore

# 4. Download the "Graphical Guy"
echo "Fetching RecoveryMore GUI from GitHub..."
curl -L $REPO_URL -o RecoveryMore.exe

if [ $? -eq 0 ]; then
    echo "Download Complete."
    echo "Launching Graphical Interface..."
    
    # 5. Launch the EXE
    # In WinRE, we can execute the EXE directly from the shell
    ./RecoveryMore.exe
else
    echo "Error: Could not reach GitHub. Check your internet connection."
fi
