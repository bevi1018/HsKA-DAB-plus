# Copyright 2011,2012 Free Software Foundation, Inc.
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
# Install public header files
########################################################################
install(FILES
    api.h
    Super_Frame_Synchronisation.h
    OFDM_Symbol_Synchronisation.h
    OFDM_coarse_frequency_correction.h
    DQPSK_Demodulation.h
    Remove_First_OFDM_Symbol.h
    Frequency_Deinterleaver.h
    Soft_Demapper.h
    Departitioning.h
    unpuncture_fic.h
    FIC_Scrambler.h
    CRC16.h
    fib_decoder.h
    radio_gui.h
    time_deinterleaver.h
    unpuncture_msc.h
    viterbi_wrapper.h
    MSC_Scrambler.h
    firecode_checker.h
    reed_solomon_decoder.h
    MP4_decoder.h DESTINATION include/HsKA_DAB_plus
)
