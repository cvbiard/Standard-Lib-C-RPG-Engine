# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\RPG.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\RPG.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\RPG.dir\flags.make

CMakeFiles\RPG.dir\main.c.obj: CMakeFiles\RPG.dir\flags.make
CMakeFiles\RPG.dir\main.c.obj: ..\main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/RPG.dir/main.c.obj"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\RPG.dir\main.c.obj /FdCMakeFiles\RPG.dir\ /FS -c C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\main.c
<<

CMakeFiles\RPG.dir\main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RPG.dir/main.c.i"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe > CMakeFiles\RPG.dir\main.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\main.c
<<

CMakeFiles\RPG.dir\main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RPG.dir/main.c.s"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\RPG.dir\main.c.s /c C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\main.c
<<

CMakeFiles\RPG.dir\protos.c.obj: CMakeFiles\RPG.dir\flags.make
CMakeFiles\RPG.dir\protos.c.obj: ..\protos.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/RPG.dir/protos.c.obj"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\RPG.dir\protos.c.obj /FdCMakeFiles\RPG.dir\ /FS -c C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\protos.c
<<

CMakeFiles\RPG.dir\protos.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RPG.dir/protos.c.i"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe > CMakeFiles\RPG.dir\protos.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\protos.c
<<

CMakeFiles\RPG.dir\protos.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RPG.dir/protos.c.s"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\RPG.dir\protos.c.s /c C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\protos.c
<<

# Object files for target RPG
RPG_OBJECTS = \
"CMakeFiles\RPG.dir\main.c.obj" \
"CMakeFiles\RPG.dir\protos.c.obj"

# External object files for target RPG
RPG_EXTERNAL_OBJECTS =

RPG.exe: CMakeFiles\RPG.dir\main.c.obj
RPG.exe: CMakeFiles\RPG.dir\protos.c.obj
RPG.exe: CMakeFiles\RPG.dir\build.make
RPG.exe: CMakeFiles\RPG.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable RPG.exe"
	"C:\Program Files\JetBrains\CLion 2021.1\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\RPG.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x86\mt.exe --manifests -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\RPG.dir\objects1.rsp @<<
 /out:RPG.exe /implib:RPG.lib /pdb:C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug\RPG.pdb /version:0.0 /machine:X86 /debug /INCREMENTAL /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\RPG.dir\build: RPG.exe

.PHONY : CMakeFiles\RPG.dir\build

CMakeFiles\RPG.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\RPG.dir\cmake_clean.cmake
.PHONY : CMakeFiles\RPG.dir\clean

CMakeFiles\RPG.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug C:\Users\Clark\Desktop\Standard-Lib-C-RPG-Engine\RPG\cmake-build-debug\CMakeFiles\RPG.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\RPG.dir\depend

