# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.1.1\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/FinalProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FinalProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FinalProject.dir/flags.make

CMakeFiles/FinalProject.dir/main.cpp.obj: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FinalProject.dir/main.cpp.obj"
	C:\MinGW64\mingw32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\FinalProject.dir\main.cpp.obj -c "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\main.cpp"

CMakeFiles/FinalProject.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/main.cpp.i"
	C:\MinGW64\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\main.cpp" > CMakeFiles\FinalProject.dir\main.cpp.i

CMakeFiles/FinalProject.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/main.cpp.s"
	C:\MinGW64\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\main.cpp" -o CMakeFiles\FinalProject.dir\main.cpp.s

# Object files for target FinalProject
FinalProject_OBJECTS = \
"CMakeFiles/FinalProject.dir/main.cpp.obj"

# External object files for target FinalProject
FinalProject_EXTERNAL_OBJECTS =

FinalProject.exe: CMakeFiles/FinalProject.dir/main.cpp.obj
FinalProject.exe: CMakeFiles/FinalProject.dir/build.make
FinalProject.exe: CMakeFiles/FinalProject.dir/linklibs.rsp
FinalProject.exe: CMakeFiles/FinalProject.dir/objects1.rsp
FinalProject.exe: CMakeFiles/FinalProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FinalProject.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\FinalProject.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FinalProject.dir/build: FinalProject.exe

.PHONY : CMakeFiles/FinalProject.dir/build

CMakeFiles/FinalProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\FinalProject.dir\cmake_clean.cmake
.PHONY : CMakeFiles/FinalProject.dir/clean

CMakeFiles/FinalProject.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject" "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject" "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\cmake-build-debug" "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\cmake-build-debug" "D:\CSCI 441\LAST_FINAL_PROJECT\FinalProject\FinalProject\cmake-build-debug\CMakeFiles\FinalProject.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/FinalProject.dir/depend

