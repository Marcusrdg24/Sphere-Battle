cmake_minimum_required(VERSION 3.28)
project(sphereBattle LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Tappa 1
add_executable(sphereBattle1 src/Tappa1/sphereBattle.cpp)
target_compile_features(sphereBattle1 PRIVATE cxx_std_17)
target_link_libraries(sphereBattle1 PRIVATE SFML::Window SFML::System SFML::Graphics)

# Tappa 2
add_executable(sphereBattle2 src/Tappa2/sphereBattle.cpp)
target_compile_features(sphereBattle2 PRIVATE cxx_std_17)
target_link_libraries(sphereBattle2 PRIVATE SFML::Window SFML::System SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sphereBattle)
