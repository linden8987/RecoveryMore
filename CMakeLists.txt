cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force Unicode and Windows stability
add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# THE FIX: Direct Compiler Flags
# This tells MSVC explicitly to search the root of vcpkg for your "include/..." path
set(VCPKG_ROOT "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
add_compile_options(/I "${VCPKG_ROOT}")

# Still adding these as backups
include_directories("${VCPKG_ROOT}")

# COMPILING EVERY FILE
add_executable(RecoveryMore 
    src/main.cpp 
    src/app.cpp
    src/browser_engine.cpp
    src/client_handler.cpp
    src/download_handler.cpp
    src/resources.rc
)

link_directories("${VCPKG_ROOT}/lib")

target_link_libraries(RecoveryMore PRIVATE 
    libcef 
    libcef_dll_wrapper
    ws2_32 shell32 user32 gdi32 ole32
)

set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
