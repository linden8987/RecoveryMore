cmake_minimum_required(VERSION 3.10)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)

# Manually point to the WebView2 include directory in vcpkg
include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")

add_executable(RecoveryMore WIN32 main.cpp)

# Link the specific WebView2 import library and Windows subsystems
target_link_libraries(RecoveryMore PRIVATE 
    WebView2Loader.dll.lib
    user32 
    gdi32 
    shell32 
    comdlg32 
    ole32
)
