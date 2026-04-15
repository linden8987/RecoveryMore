cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# 1. Manually define the paths since find_package(cef) failed
set(CEF_ROOT "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
include_directories("${CEF_ROOT}/include")

# 2. Add all your implementation files
add_executable(RecoveryMore 
    src/main.cpp 
    src/app.cpp
    src/browser_engine.cpp
    src/client_handler.cpp
    src/download_handler.cpp
    src/resources.rc
)

# 3. Explicitly link the libraries from the vcpkg lib folder
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
