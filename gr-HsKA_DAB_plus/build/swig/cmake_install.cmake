# Install script for directory: /home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/swig

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus/_HsKA_DAB_plus_swig.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus/_HsKA_DAB_plus_swig.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus/_HsKA_DAB_plus_swig.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus" TYPE MODULE FILES "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/swig/_HsKA_DAB_plus_swig.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus/_HsKA_DAB_plus_swig.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus/_HsKA_DAB_plus_swig.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus/_HsKA_DAB_plus_swig.so"
         OLD_RPATH "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus/_HsKA_DAB_plus_swig.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus" TYPE FILE FILES "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/swig/HsKA_DAB_plus_swig.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages/HsKA_DAB_plus" TYPE FILE FILES
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/swig/HsKA_DAB_plus_swig.pyc"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/swig/HsKA_DAB_plus_swig.pyo"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/HsKA_DAB_plus/HsKA_DAB_plus/swig" TYPE FILE FILES
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/swig/HsKA_DAB_plus_swig.i"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/swig/HsKA_DAB_plus_swig_doc.i"
    )
endif()

