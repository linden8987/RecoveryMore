cmake_minimum_required(VERSION 3.10)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)

# Find WebView2 from the vcpkg installation
find_package(webview2 CONFIG REQUIRED)

add_executable(RecoveryMore WIN32 main.cpp)

# Linking the Chromium, UI, and File System libraries
target_link_libraries(RecoveryMore PRIVATE 
    webview2::Microsoft.Web.WebView2 
    user32 
    gdi32 
    shell32 
    comdlg32 
    ole32
)
