# Install script for directory: /home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/gnuradiouser/gnuradio/installation/default")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/HsKA_DAB_plus" TYPE FILE FILES
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/api.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/Super_Frame_Synchronisation.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/OFDM_Symbol_Synchronisation.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/OFDM_coarse_frequency_correction.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/DQPSK_Demodulation.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/Remove_First_OFDM_Symbol.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/Frequency_Deinterleaver.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/Soft_Demapper.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/Departitioning.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/unpuncture_fic.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/FIC_Scrambler.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/CRC16.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/fib_decoder.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/radio_gui.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/time_deinterleaver.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/unpuncture_msc.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/viterbi_wrapper.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/MSC_Scrambler.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/firecode_checker.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/reed_solomon_decoder.h"
    "/home/gnuradiouser/gnuradio/HsKA-DAB-plus/gr-HsKA_DAB_plus/include/HsKA_DAB_plus/MP4_decoder.h"
    )
endif()

