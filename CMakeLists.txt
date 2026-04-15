cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force Static Linking (This is what makes it "No DLLs")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Find your VCPKG packages here (e.g., your browser engine)
# find_package(CEF REQUIRED) 

add_executable(RecoveryMore WIN32
    src/main.cpp
    # Add your other files here so they compile
    # src/browser_engine.cpp
    # src/file_explorer.cpp
)

# Tells the compiler where to find your .h files
target_include_directories(RecoveryMore PRIVATE src)

# Link Windows System Libraries
target_link_libraries(RecoveryMore PRIVATE 
    Shlwapi.lib 
    Shell32.lib 
    User32.lib 
    Gdi32.lib 
    Advapi32.lib
)
