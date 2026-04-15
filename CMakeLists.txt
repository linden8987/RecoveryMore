# CMakeLists.txt Configuration for CEF Static Build
cmake_minimum_required(VERSION 3.10)
project(RecoveryMore)

# Set CEF options
set(CEF_USE_STATIC true)
set(CEF_USE_LIBS libcef.a)
set(CEF_USE_TAB_SUPPORT true)
set(CEF_USE_DOWNLOADS true)
set(CEF_USE_FILE_EXPLORER true)

# Add other necessary CEF configuration
add_definitions(-DUSE_CEF_STATIC)
include_directories(${CEF_INCLUDE_DIR})

# Specify the source files and include directories
set(SOURCES
    main.cpp
    ... // other source files
)

add_executable(RecoveryMore ${SOURCES})
target_link_libraries(RecoveryMore ${CEF_USE_LIBS})

# Additional configurations for embedding CEF
set_target_properties(RecoveryMore PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

# Specify any needed components for static linking
#... (further configuration relevant to your project)
