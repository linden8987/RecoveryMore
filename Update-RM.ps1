# Update-RM.ps1
$repo = "linden8987/RecoveryMore"
$exeName = "RecoveryMore.exe"
$localPath = "R:\Tools\$exeName"

# 1. Get the latest release info from GitHub API
$release = Invoke-RestMethod -Uri "https://api.github.com/repos/$repo/releases/latest"
$latestUrl = ($release.assets | Where-Object { $_.name -like "*$exeName*" }).browser_download_url

Write-Host "Checking for updates..." -ForegroundColor Cyan

# 2. Download the latest version
Invoke-WebRequest -Uri $latestUrl -OutFile "$localPath.tmp"

# 3. Swap the files (Atomic move)
Move-Item -Path "$localPath.tmp" -Destination $localPath -Force

Write-Host "Update Complete! Launching RecoveryMore..." -ForegroundColor Green

# 4. Launch the tool
Start-Process -FilePath $localPath
