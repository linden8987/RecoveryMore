cmake_minimum_required(VERSION 3.15)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MSVC to ignore header conflicts
add_definitions(-DWIN32_LEAN_AND_MEAN)

# Locate WebView2 via vcpkg
find_package(WebView2 REQUIRED)

add_executable(RecoveryMore src/main.cpp)

# Link the libraries and the include directories
target_link_libraries(RecoveryMore PRIVATE 
    WebView2::WebView2
    ws2_32 
    shell32
)

# Set as a Windows GUI app (hides the console)
set_target_properties(RecoveryMore PROPERTIES WIN32_EXECUTABLE TRUE)
