# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build

# Utility rule file for HsKA_DAB_plus_swig_swig_doc.

# Include the progress variables for this target.
include swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/progress.make

swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc: swig/HsKA_DAB_plus_swig_doc.i


swig/HsKA_DAB_plus_swig_doc.i: swig/HsKA_DAB_plus_swig_doc_swig_docs/xml/index.xml
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating python docstrings for HsKA_DAB_plus_swig_doc"
	cd /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/docs/doxygen && /usr/bin/python2 -B /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/docs/doxygen/swig_doc.py /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig/HsKA_DAB_plus_swig_doc_swig_docs/xml /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig/HsKA_DAB_plus_swig_doc.i

swig/HsKA_DAB_plus_swig_doc_swig_docs/xml/index.xml: swig/_HsKA_DAB_plus_swig_doc_tag
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating doxygen xml for HsKA_DAB_plus_swig_doc docs"
	cd /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig && ./_HsKA_DAB_plus_swig_doc_tag
	cd /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig && /usr/bin/doxygen /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig/HsKA_DAB_plus_swig_doc_swig_docs/Doxyfile

HsKA_DAB_plus_swig_swig_doc: swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc
HsKA_DAB_plus_swig_swig_doc: swig/HsKA_DAB_plus_swig_doc.i
HsKA_DAB_plus_swig_swig_doc: swig/HsKA_DAB_plus_swig_doc_swig_docs/xml/index.xml
HsKA_DAB_plus_swig_swig_doc: swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/build.make

.PHONY : HsKA_DAB_plus_swig_swig_doc

# Rule to build all files generated by this target.
swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/build: HsKA_DAB_plus_swig_swig_doc

.PHONY : swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/build

swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/clean:
	cd /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/clean

swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/depend:
	cd /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/swig /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/build/swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/HsKA_DAB_plus_swig_swig_doc.dir/depend

