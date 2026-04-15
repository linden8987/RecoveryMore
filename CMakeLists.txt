cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Standard Windows definitions
add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# 2. Find CEF via vcpkg
# This looks for the wrapper files provided by the vcpkg port
find_package(cef CONFIG REQUIRED)

# 3. Add all your source files
add_executable(RecoveryMore 
    src/main.cpp 
    src/app.cpp
    src/browser_engine.cpp
    src/client_handler.cpp
    src/download_handler.cpp
    src/resources.rc
)

# 4. Link CEF and its required system libs
# cef::libcef is the target name used by the vcpkg config
target_link_libraries(RecoveryMore PRIVATE 
    cef::libcef
    ws2_32 
    shell32 
    user32 
    gdi32 
    ole32
)

# 5. Build as a Windows GUI app
set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
