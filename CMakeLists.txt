cmake_minimum_required(VERSION 3.10)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)

# This forces Static Linking of the C++ Runtime (removes VCRUNTIME140.dll dependency)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")

add_executable(RecoveryMore WIN32 main.cpp)

target_link_libraries(RecoveryMore PRIVATE 
    "C:/vcpkg/installed/x64-windows/lib/WebView2Loader.dll.lib"
    user32 
    gdi32 
    shell32 
    comdlg32 
    ole32
)
