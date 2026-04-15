cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Unicode and prevent header bloat
add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# CEF and VCPKG Paths
include_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
link_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")

# Include all files from your src directory
add_executable(RecoveryMore 
    src/main.cpp 
    src/app.cpp
    src/browser_engine.cpp
    src/client_handler.cpp
    src/download_handler.cpp
    src/resources.rc
)

# Link the CEF libraries and Windows system tools
target_link_libraries(RecoveryMore PRIVATE 
    libcef 
    libcef_dll_wrapper
    ws2_32 
    shell32
    user32
    gdi32
)

# Build as a Windows GUI app (hides the console)
set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
