cmake_minimum_required(VERSION 3.10)
project(URLShortner)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add the executable
add_executable(URLShortner ${SOURCES})

# Link libraries
target_link_libraries(URLShortner PRIVATE OpenSSL::Crypto)
