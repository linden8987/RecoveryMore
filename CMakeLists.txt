cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# POINTING TO THE TRIPLET ROOT
# This allows #include "include/cef_app.h" to resolve correctly
set(CEF_ROOT "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
include_directories("${CEF_ROOT}")

# COMPILING EVERY FILE IN YOUR LIST
add_executable(RecoveryMore 
    src/main.cpp 
    src/app.cpp
    src/browser_engine.cpp
    src/client_handler.cpp
    src/download_handler.cpp
    src/resources.rc
)

# Linker paths for the libraries
link_directories("${CEF_ROOT}/lib")

target_link_libraries(RecoveryMore PRIVATE 
    libcef 
    libcef_dll_wrapper
    ws2_32 
    shell32 
    user32 
    gdi32 
    ole32
)

set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
