cmake_minimum_required(VERSION 3.12)
#project(MP)
#Vlad's Project
project(FinalProject)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp)
#add_executable(MP ${SOURCE_FILES})
#Vlad's Executable
add_executable(FinalProject ${SOURCE_FILES})

######
# If you are on the Lab Machines, or have installed the OpenGL libraries somewhere
# other than on your path, leave the following two lines uncommented and update
# the path.  The current path is for the lab environments.
#
# If your installations are on your path, then comment these two lines
######

#include_directories("Z:/CSCI441/include")
#target_link_directories(MP PUBLIC "Z:/CSCI441/lib")

# the following line is linking instructions for Windows.  comment if on OS X, otherwise leave uncommented
#target_link_libraries(MP opengl32 glfw3 glew32.dll gdi32)
#Vlad's libraries
target_link_libraries(FinalProject opengl32 glfw3 glew32.dll gdi32 stbimage)

# the following line is linking instructions for OS X.  uncomment if on OS X, otherwise leave commented
#target_link_libraries(lab02 "-framework OpenGL" glfw3 "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" glew)