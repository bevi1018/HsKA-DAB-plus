# Install script for directory: /home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_Super_Frame_Synchronisation.xml"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_OFDM_Symbol_Synchronisation.xml"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_OFDM_coarse_frequency_correction.xml"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_DQPSK_Demodulation.xml"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_Remove_First_OFDM_Symbol.xml"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_Frequency_Deinterleaver.xml"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_Soft_Demapper.xml"
    "/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/grc/HsKA_DAB_plus_Departitioning.xml"
    )
endif()

