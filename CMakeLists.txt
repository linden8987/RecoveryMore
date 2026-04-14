cmake_minimum_required(VERSION 3.10)
project(RecoveryMore)

add_executable(RecoveryMore WIN32 main.cpp)

# Linking all required modules for GUI, Explorer, and Themes
target_link_libraries(RecoveryMore user32 gdi32 shell32 comdlg32 ole32)
