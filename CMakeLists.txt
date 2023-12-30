cmake_minimum_required(VERSION 3.1)
project(Chronos)

set(CMAKE_CXX_STANDARD 20)

#GLFW
add_subdirectory(ThirdParty/glfw)
set(GLFW_DIR ThirdParty/glfw/include)

#vulkan sdk if using windows
if (WIN32)
set(VULKAN_SDK_DIR $ENV{VULKAN_SDK})        
set(VULKAN_SDK_LIB ${VULKAN_SDK_DIR}/Lib/)                  
set(VULKAN_SDK_INCLUDE ${VULKAN_SDK_DIR}/Include) 
endif (WIN32)

#imgui
set(IMGUI_DIR ThirdParty/imgui) 
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)

#fonts
set(FONTS_DIR ThirdParty/fonts/usascii/arial)
set(FONTS_DIR ThirdParty/fonts/latin1/consolas)


#header files
include_directories(ThirdParty/stb)
include_directories(Source/Engine/Headers)
include_directories(include)
include_directories(Source/API/Headers)
include_directories(${GLFW_DIR})  
include_directories(${IMGUI_DIR})               
include_directories(${IMGUI_BACKENDS})
include_directories(${FONTS_DIR})

#vulkan sdk if using windows
if(WIN32)
include_directories(${VULKAN_SDK_INCLUDE})     
endif(WIN32) 

#source files
set(SOURCE_DIR Source/Engine/Source)
set(API_DIR Source/API/Source)
file(GLOB MAIN ${SOURCE_DIR}/*.cpp)
file(GLOB API ${API_DIR}/*.cpp)
file(GLOB IMGUI ${IMGUI_DIR}/*.cpp)

#again windows vulkan stuf....git 
if(WIN32)
link_directories(${VULKAN_SDK_LIB})     
endif(WIN32)

#linking
add_library(Chronos ${MAIN} ${API} ${IMGUI} ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp ${IMGUI_BACKENDS}/imgui_impl_vulkan.cpp)
# add_executable(Chronos ${MAIN} ${API} ${IMGUI} ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp ${IMGUI_BACKENDS}/imgui_impl_vulkan.cpp)      

#linking vulkan is a pain....
if(WIN32)
target_link_libraries(Chronos vulkan-1) 
endif(WIN32)
if(UNIX)
target_link_libraries(Chronos vulkan)   
endif(UNIX)

target_link_libraries(Chronos glfw)