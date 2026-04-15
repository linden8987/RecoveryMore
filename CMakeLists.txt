cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DWIN32_LEAN_AND_MEAN)

# Direct pathing for GitHub Actions / vcpkg environments
include_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
link_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")

add_executable(RecoveryMore src/main.cpp)

# Link the libraries directly
target_link_libraries(RecoveryMore PRIVATE 
    WebView2Loader
    ws2_32 
    shell32
)

set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
