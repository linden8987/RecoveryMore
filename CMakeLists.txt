cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Unicode and Header protection
add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# Set the base paths for vcpkg
set(VCPKG_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
set(VCPKG_LIB_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")

include_directories("${VCPKG_INCLUDE_DIR}")
link_directories("${VCPKG_LIB_DIR}")

# This part is the fix: It searches for the actual .lib file location
find_library(WEBVIEW2_LIB NAMES WebView2Loader PATHS "${VCPKG_LIB_DIR}" REQUIRED)

add_executable(RecoveryMore src/main.cpp)

# Link using the found path
target_link_libraries(RecoveryMore PRIVATE 
    "${WEBVIEW2_LIB}"
    ws2_32 
    shell32
)

set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
