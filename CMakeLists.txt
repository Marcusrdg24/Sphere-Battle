cmake_minimum_required(VERSION 4.0)
project(sphereBattle LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum policy version for all subprojects")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(common INTERFACE)
target_compile_features(common INTERFACE cxx_std_17)
target_compile_options(common INTERFACE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
  $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

# Tappa 1
add_executable(sphereBattle1 Tappa1/sphereBattle.cpp)
target_link_libraries(sphereBattle1 PRIVATE SFML::Window SFML::System SFML::Graphics)

# Tappa 2
add_executable(sphereBattle2 Tappa2/sphereBattle.cpp)
target_link_libraries(sphereBattle2 PRIVATE SFML::Window SFML::System SFML::Graphics)

# Tappa 3
add_executable(sphereBattle3 Tappa3/sphereBattle.cpp)
target_link_libraries(sphereBattle3 PRIVATE SFML::Window SFML::System SFML::Graphics)