# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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
<<<<<<< HEAD
CMAKE_COMMAND = /home/wyj/Downloads/clion-2019.1.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/wyj/Downloads/clion-2019.1.4/bin/cmake/linux/bin/cmake -E remove -f
=======
CMAKE_COMMAND = "D:\wyj\CLion\CLion 2019.1.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\wyj\CLion\CLion 2019.1.4\bin\cmake\win\bin\cmake.exe" -E remove -f
>>>>>>> 9c2fc85bbcde814a2865a1125a9371d4283235e4

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
<<<<<<< HEAD
CMAKE_SOURCE_DIR = /home/wyj/Desktop/VFS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wyj/Desktop/VFS/cmake-build-debug
=======
CMAKE_SOURCE_DIR = C:\Users\30803\Desktop\VFS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\30803\Desktop\VFS\cmake-build-debug
>>>>>>> 9c2fc85bbcde814a2865a1125a9371d4283235e4

# Include any dependencies generated for this target.
include CMakeFiles/MyFileSystem.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MyFileSystem.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyFileSystem.dir/flags.make

<<<<<<< HEAD
CMakeFiles/MyFileSystem.dir/main.cpp.o: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wyj/Desktop/VFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyFileSystem.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyFileSystem.dir/main.cpp.o -c /home/wyj/Desktop/VFS/main.cpp

CMakeFiles/MyFileSystem.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wyj/Desktop/VFS/main.cpp > CMakeFiles/MyFileSystem.dir/main.cpp.i

CMakeFiles/MyFileSystem.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wyj/Desktop/VFS/main.cpp -o CMakeFiles/MyFileSystem.dir/main.cpp.s

CMakeFiles/MyFileSystem.dir/FileSystem.cpp.o: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/FileSystem.cpp.o: ../FileSystem.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wyj/Desktop/VFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MyFileSystem.dir/FileSystem.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyFileSystem.dir/FileSystem.cpp.o -c /home/wyj/Desktop/VFS/FileSystem.cpp

CMakeFiles/MyFileSystem.dir/FileSystem.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/FileSystem.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wyj/Desktop/VFS/FileSystem.cpp > CMakeFiles/MyFileSystem.dir/FileSystem.cpp.i

CMakeFiles/MyFileSystem.dir/FileSystem.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/FileSystem.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wyj/Desktop/VFS/FileSystem.cpp -o CMakeFiles/MyFileSystem.dir/FileSystem.cpp.s

CMakeFiles/MyFileSystem.dir/BlockManager.cpp.o: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/BlockManager.cpp.o: ../BlockManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wyj/Desktop/VFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MyFileSystem.dir/BlockManager.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyFileSystem.dir/BlockManager.cpp.o -c /home/wyj/Desktop/VFS/BlockManager.cpp

CMakeFiles/MyFileSystem.dir/BlockManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/BlockManager.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wyj/Desktop/VFS/BlockManager.cpp > CMakeFiles/MyFileSystem.dir/BlockManager.cpp.i

CMakeFiles/MyFileSystem.dir/BlockManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/BlockManager.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wyj/Desktop/VFS/BlockManager.cpp -o CMakeFiles/MyFileSystem.dir/BlockManager.cpp.s

CMakeFiles/MyFileSystem.dir/FileManager.cpp.o: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/FileManager.cpp.o: ../FileManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wyj/Desktop/VFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MyFileSystem.dir/FileManager.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyFileSystem.dir/FileManager.cpp.o -c /home/wyj/Desktop/VFS/FileManager.cpp

CMakeFiles/MyFileSystem.dir/FileManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/FileManager.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wyj/Desktop/VFS/FileManager.cpp > CMakeFiles/MyFileSystem.dir/FileManager.cpp.i

CMakeFiles/MyFileSystem.dir/FileManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/FileManager.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wyj/Desktop/VFS/FileManager.cpp -o CMakeFiles/MyFileSystem.dir/FileManager.cpp.s

CMakeFiles/MyFileSystem.dir/FileTree.cpp.o: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/FileTree.cpp.o: ../FileTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wyj/Desktop/VFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/MyFileSystem.dir/FileTree.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyFileSystem.dir/FileTree.cpp.o -c /home/wyj/Desktop/VFS/FileTree.cpp

CMakeFiles/MyFileSystem.dir/FileTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/FileTree.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wyj/Desktop/VFS/FileTree.cpp > CMakeFiles/MyFileSystem.dir/FileTree.cpp.i

CMakeFiles/MyFileSystem.dir/FileTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/FileTree.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wyj/Desktop/VFS/FileTree.cpp -o CMakeFiles/MyFileSystem.dir/FileTree.cpp.s
=======
CMakeFiles/MyFileSystem.dir/main.cpp.obj: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\30803\Desktop\VFS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyFileSystem.dir/main.cpp.obj"
	C:\mingw32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MyFileSystem.dir\main.cpp.obj -c C:\Users\30803\Desktop\VFS\main.cpp

CMakeFiles/MyFileSystem.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/main.cpp.i"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\30803\Desktop\VFS\main.cpp > CMakeFiles\MyFileSystem.dir\main.cpp.i

CMakeFiles/MyFileSystem.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/main.cpp.s"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\30803\Desktop\VFS\main.cpp -o CMakeFiles\MyFileSystem.dir\main.cpp.s

CMakeFiles/MyFileSystem.dir/FileSystem.cpp.obj: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/FileSystem.cpp.obj: ../FileSystem.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\30803\Desktop\VFS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MyFileSystem.dir/FileSystem.cpp.obj"
	C:\mingw32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MyFileSystem.dir\FileSystem.cpp.obj -c C:\Users\30803\Desktop\VFS\FileSystem.cpp

CMakeFiles/MyFileSystem.dir/FileSystem.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/FileSystem.cpp.i"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\30803\Desktop\VFS\FileSystem.cpp > CMakeFiles\MyFileSystem.dir\FileSystem.cpp.i

CMakeFiles/MyFileSystem.dir/FileSystem.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/FileSystem.cpp.s"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\30803\Desktop\VFS\FileSystem.cpp -o CMakeFiles\MyFileSystem.dir\FileSystem.cpp.s

CMakeFiles/MyFileSystem.dir/BlockManager.cpp.obj: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/BlockManager.cpp.obj: ../BlockManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\30803\Desktop\VFS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MyFileSystem.dir/BlockManager.cpp.obj"
	C:\mingw32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MyFileSystem.dir\BlockManager.cpp.obj -c C:\Users\30803\Desktop\VFS\BlockManager.cpp

CMakeFiles/MyFileSystem.dir/BlockManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/BlockManager.cpp.i"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\30803\Desktop\VFS\BlockManager.cpp > CMakeFiles\MyFileSystem.dir\BlockManager.cpp.i

CMakeFiles/MyFileSystem.dir/BlockManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/BlockManager.cpp.s"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\30803\Desktop\VFS\BlockManager.cpp -o CMakeFiles\MyFileSystem.dir\BlockManager.cpp.s

CMakeFiles/MyFileSystem.dir/FileManager.cpp.obj: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/FileManager.cpp.obj: ../FileManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\30803\Desktop\VFS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MyFileSystem.dir/FileManager.cpp.obj"
	C:\mingw32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MyFileSystem.dir\FileManager.cpp.obj -c C:\Users\30803\Desktop\VFS\FileManager.cpp

CMakeFiles/MyFileSystem.dir/FileManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/FileManager.cpp.i"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\30803\Desktop\VFS\FileManager.cpp > CMakeFiles\MyFileSystem.dir\FileManager.cpp.i

CMakeFiles/MyFileSystem.dir/FileManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/FileManager.cpp.s"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\30803\Desktop\VFS\FileManager.cpp -o CMakeFiles\MyFileSystem.dir\FileManager.cpp.s

CMakeFiles/MyFileSystem.dir/FileTree.cpp.obj: CMakeFiles/MyFileSystem.dir/flags.make
CMakeFiles/MyFileSystem.dir/FileTree.cpp.obj: ../FileTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\30803\Desktop\VFS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/MyFileSystem.dir/FileTree.cpp.obj"
	C:\mingw32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MyFileSystem.dir\FileTree.cpp.obj -c C:\Users\30803\Desktop\VFS\FileTree.cpp

CMakeFiles/MyFileSystem.dir/FileTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyFileSystem.dir/FileTree.cpp.i"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\30803\Desktop\VFS\FileTree.cpp > CMakeFiles\MyFileSystem.dir\FileTree.cpp.i

CMakeFiles/MyFileSystem.dir/FileTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyFileSystem.dir/FileTree.cpp.s"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\30803\Desktop\VFS\FileTree.cpp -o CMakeFiles\MyFileSystem.dir\FileTree.cpp.s
>>>>>>> 9c2fc85bbcde814a2865a1125a9371d4283235e4

# Object files for target MyFileSystem
MyFileSystem_OBJECTS = \
"CMakeFiles/MyFileSystem.dir/main.cpp.obj" \
"CMakeFiles/MyFileSystem.dir/FileSystem.cpp.obj" \
"CMakeFiles/MyFileSystem.dir/BlockManager.cpp.obj" \
"CMakeFiles/MyFileSystem.dir/FileManager.cpp.obj" \
"CMakeFiles/MyFileSystem.dir/FileTree.cpp.obj"

# External object files for target MyFileSystem
MyFileSystem_EXTERNAL_OBJECTS =

<<<<<<< HEAD
MyFileSystem: CMakeFiles/MyFileSystem.dir/main.cpp.o
MyFileSystem: CMakeFiles/MyFileSystem.dir/FileSystem.cpp.o
MyFileSystem: CMakeFiles/MyFileSystem.dir/BlockManager.cpp.o
MyFileSystem: CMakeFiles/MyFileSystem.dir/FileManager.cpp.o
MyFileSystem: CMakeFiles/MyFileSystem.dir/FileTree.cpp.o
MyFileSystem: CMakeFiles/MyFileSystem.dir/build.make
MyFileSystem: CMakeFiles/MyFileSystem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wyj/Desktop/VFS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable MyFileSystem"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyFileSystem.dir/link.txt --verbose=$(VERBOSE)
=======
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/main.cpp.obj
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/FileSystem.cpp.obj
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/BlockManager.cpp.obj
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/FileManager.cpp.obj
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/FileTree.cpp.obj
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/build.make
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/linklibs.rsp
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/objects1.rsp
MyFileSystem.exe: CMakeFiles/MyFileSystem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\30803\Desktop\VFS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable MyFileSystem.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MyFileSystem.dir\link.txt --verbose=$(VERBOSE)
>>>>>>> 9c2fc85bbcde814a2865a1125a9371d4283235e4

# Rule to build all files generated by this target.
CMakeFiles/MyFileSystem.dir/build: MyFileSystem.exe

.PHONY : CMakeFiles/MyFileSystem.dir/build

CMakeFiles/MyFileSystem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MyFileSystem.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MyFileSystem.dir/clean

CMakeFiles/MyFileSystem.dir/depend:
<<<<<<< HEAD
	cd /home/wyj/Desktop/VFS/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wyj/Desktop/VFS /home/wyj/Desktop/VFS /home/wyj/Desktop/VFS/cmake-build-debug /home/wyj/Desktop/VFS/cmake-build-debug /home/wyj/Desktop/VFS/cmake-build-debug/CMakeFiles/MyFileSystem.dir/DependInfo.cmake --color=$(COLOR)
=======
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\30803\Desktop\VFS C:\Users\30803\Desktop\VFS C:\Users\30803\Desktop\VFS\cmake-build-debug C:\Users\30803\Desktop\VFS\cmake-build-debug C:\Users\30803\Desktop\VFS\cmake-build-debug\CMakeFiles\MyFileSystem.dir\DependInfo.cmake --color=$(COLOR)
>>>>>>> 9c2fc85bbcde814a2865a1125a9371d4283235e4
.PHONY : CMakeFiles/MyFileSystem.dir/depend

