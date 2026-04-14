cmake_minimum_required(VERSION 3.10)
project(RecoveryMore)

set(CMAKE_CXX_STANDARD 17)

# Build a Windows GUI app (hides the console window)
add_executable(RecoveryMore WIN32 main.cpp)
target_link_libraries(RecoveryMore user32 gdi32)
