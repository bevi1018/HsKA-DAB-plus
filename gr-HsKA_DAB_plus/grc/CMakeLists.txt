# Copyright 2011 Free Software Foundation, Inc.
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

install(FILES
    HsKA_DAB_plus_Super_Frame_Synchronisation.xml
    HsKA_DAB_plus_OFDM_Symbol_Synchronisation.xml
    HsKA_DAB_plus_OFDM_coarse_frequency_correction.xml
    HsKA_DAB_plus_DQPSK_Demodulation.xml
    HsKA_DAB_plus_Remove_First_OFDM_Symbol.xml
    HsKA_DAB_plus_Frequency_Deinterleaver.xml
    HsKA_DAB_plus_Soft_Demapper.xml
    HsKA_DAB_plus_Departitioning.xml DESTINATION share/gnuradio/grc/blocks
)
