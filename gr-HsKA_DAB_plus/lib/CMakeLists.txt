# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND HsKA_DAB_plus_sources
    Super_Frame_Synchronisation_impl.cc
    OFDM_Symbol_Synchronisation_impl.cc
    OFDM_coarse_frequency_correction_impl.cc
    DQPSK_Demodulation_impl.cc
    Remove_First_OFDM_Symbol_impl.cc
    Frequency_Deinterleaver_impl.cc
    Soft_Demapper_impl.cc
)

set(HsKA_DAB_plus_sources "${HsKA_DAB_plus_sources}" PARENT_SCOPE)
if(NOT HsKA_DAB_plus_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT HsKA_DAB_plus_sources)

add_library(gnuradio-HsKA_DAB_plus SHARED ${HsKA_DAB_plus_sources})
target_link_libraries(gnuradio-HsKA_DAB_plus ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-HsKA_DAB_plus PROPERTIES DEFINE_SYMBOL "gnuradio_HsKA_DAB_plus_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-HsKA_DAB_plus PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-HsKA_DAB_plus RUNTIME_COMPONENT "HsKA_DAB_plus_runtime" DEVEL_COMPONENT "HsKA_DAB_plus_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_HsKA_DAB_plus_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_HsKA_DAB_plus.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_HsKA_DAB_plus.cc
)

add_executable(test-HsKA_DAB_plus ${test_HsKA_DAB_plus_sources})

target_link_libraries(
  test-HsKA_DAB_plus
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-HsKA_DAB_plus
)

GR_ADD_TEST(test_HsKA_DAB_plus test-HsKA_DAB_plus)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

