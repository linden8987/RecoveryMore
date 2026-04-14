cmake_minimum_required(VERSION 3.10)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)

# The "vcpkg way" to find webview2 if CONFIG fails
find_package(webview2 REQUIRED)

add_executable(RecoveryMore WIN32 main.cpp)

# Link using the standard variable names vcpkg provides
target_link_libraries(RecoveryMore PRIVATE 
    webview2
    user32 
    gdi32 
    shell32 
    comdlg32 
    ole32
)
