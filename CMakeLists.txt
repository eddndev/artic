cmake_minimum_required(VERSION 3.20)
project(Artic VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(ARTIC_BUILD_TESTS "Build test suite" ON)
option(ARTIC_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(ARTIC_BUILD_TOOLS "Build LSP and dev server" OFF)
option(ARTIC_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ARTIC_ENABLE_TSAN "Enable ThreadSanitizer" OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Sanitizers
if(ARTIC_ENABLE_ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

if(ARTIC_ENABLE_TSAN)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

# Dependencies
# Uncomment when using vcpkg:
# find_package(Boost REQUIRED COMPONENTS system)
# find_package(spdlog REQUIRED)
# find_package(CLI11 REQUIRED)
# find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/runtime/server/include)

# Subdirectories
add_subdirectory(src)

if(ARTIC_BUILD_TESTS)
    enable_testing()
    # Uncomment when Google Test is available:
    # add_subdirectory(third_party/googletest)
    add_subdirectory(tests)
endif()

if(ARTIC_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(ARTIC_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# Installation
install(TARGETS artic
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY runtime/server/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

install(DIRECTORY runtime/client/dist/
    DESTINATION share/artic/runtime
    FILES_MATCHING PATTERN "*.js"
)

# CPack configuration
set(CPACK_PACKAGE_NAME "Artic")
set(CPACK_PACKAGE_VENDOR "Achronyme")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Artic - Unified Web Development Language")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

include(CPack)
