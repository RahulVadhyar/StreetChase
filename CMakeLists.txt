cmake_minimum_required(VERSION 3.13)
project(StreetChase)

set(CMAKE_CXX_STANDARD 20)

set(CHRONOS_INCLUDE_DIR ThirdParty/Chronos/include)
include_directories(BEFORE ${CHRONOS_INCLUDE_DIR})
include_directories(Source/Headers)

file(GLOB SOURCE_FILES Source/Source/*.cpp)
add_executable(StreetChase ${SOURCE_FILES})
add_subdirectory(ThirdParty/Chronos)
target_link_libraries(StreetChase Chronos)