# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad

# Utility rule file for glad-generate-files.

# Include the progress variables for this target.
include CMakeFiles/glad-generate-files.dir/progress.make

CMakeFiles/glad-generate-files: src/glad.c
CMakeFiles/glad-generate-files: include/glad/glad.h


src/glad.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating GLAD"
	/usr/bin/python -m glad --profile=compatibility --out-path=/home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad --api= --generator=c --extensions= --spec=gl

include/glad/glad.h: src/glad.c
	@$(CMAKE_COMMAND) -E touch_nocreate include/glad/glad.h

glad-generate-files: CMakeFiles/glad-generate-files
glad-generate-files: src/glad.c
glad-generate-files: include/glad/glad.h
glad-generate-files: CMakeFiles/glad-generate-files.dir/build.make

.PHONY : glad-generate-files

# Rule to build all files generated by this target.
CMakeFiles/glad-generate-files.dir/build: glad-generate-files

.PHONY : CMakeFiles/glad-generate-files.dir/build

CMakeFiles/glad-generate-files.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glad-generate-files.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glad-generate-files.dir/clean

CMakeFiles/glad-generate-files.dir/depend:
	cd /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad /home/ferramenta/Documents/3º2º/CG/Asteroids/Asteroids/Vendor/glad/CMakeFiles/glad-generate-files.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glad-generate-files.dir/depend

