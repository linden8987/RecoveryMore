cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Unicode support and ignore legacy header bloat
add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# Tell the compiler where the GitHub runner stores vcpkg headers/libs
include_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
link_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")

add_executable(RecoveryMore src/main.cpp)

# Link essential Windows and WebView2 libraries
target_link_libraries(RecoveryMore PRIVATE 
    WebView2Loader
    ws2_32 
    shell32
)

# Build as a GUI application (no background console window)
set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
