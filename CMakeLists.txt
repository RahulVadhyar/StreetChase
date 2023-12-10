cmake_minimum_required(VERSION 3.1)
project(Chronos)

set(CMAKE_CXX_STANDARD 20)

#GLFW
set(GLFW_DIR ThirdParty/glfw-3.3.8.bin.WIN64/include) # this path needed to be changed for linux, it is for windows only
set(GLFW_LIB ThirdParty/glfw-3.3.8.bin.WIN64/lib-vc2022/)   #same as above

#vulkan sdk
set(VULKAN_SDK_DIR $ENV{VULKAN_SDK})        
set(VULKAN_SDK_LIB ${VULKAN_SDK_DIR}/Lib/)                  
set(VULKAN_SDK_INCLUDE ${VULKAN_SDK_DIR}/Include) #glm is also included

#imgui
set(IMGUI_DIR ThirdParty/imgui) 
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)

#fonts
set(FONTS_DIR ThirdParty/fonts/usascii/arial)
set(FONTS_DIR ThirdParty/fonts/latin1/consolas)

#header files
include_directories(ThirdParty/stb)
include_directories(Source/Engine/Header)
include_directories(include)
include_directories(${GLFW_DIR})
include_directories(${VULKAN_SDK_INCLUDE})      
include_directories(${IMGUI_DIR})               
include_directories(${IMGUI_BACKENDS})
include_directories(${FONTS_DIR})

#source files
set(SOURCE_DIR Source/Engine/Source)
set(API_DIR Source/API)
file(GLOB MAIN ${SOURCE_DIR}/*.cpp)
file(GLOB API ${API_DIR}/*.cpp)
file(GLOB IMGUI ${IMGUI_DIR}/*.cpp)

link_directories(${GLFW_LIB})   
link_directories(${VULKAN_SDK_LIB})     

#linking
add_executable(Chronos ${MAIN} ${API} ${IMGUI} ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp ${IMGUI_BACKENDS}/imgui_impl_vulkan.cpp)      
target_link_libraries(Chronos glfw3)   
target_link_libraries(Chronos vulkan-1)           
