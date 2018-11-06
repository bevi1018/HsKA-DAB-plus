/* -*- c++ -*- */

#define HSKA_DAB_PLUS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "HsKA_DAB_plus_swig_doc.i"

%module(directors="1") HsKA_DAB_plus
%{
#include "HsKA_DAB_plus/Super_Frame_Synchronisation.h"
#include "HsKA_DAB_plus/OFDM_Symbol_Synchronisation.h"
#include "HsKA_DAB_plus/OFDM_coarse_frequency_correction.h"
#include "HsKA_DAB_plus/DQPSK_Demodulation.h"
#include "HsKA_DAB_plus/Remove_First_OFDM_Symbol.h"
#include "HsKA_DAB_plus/Frequency_Deinterleaver.h"
#include "HsKA_DAB_plus/Soft_Demapper.h"
#include "HsKA_DAB_plus/Departitioning.h"
#include "HsKA_DAB_plus/unpuncture_fic.h"
#include "HsKA_DAB_plus/FIC_Scrambler.h"
#include "HsKA_DAB_plus/CRC16.h"
#include "HsKA_DAB_plus/fib_decoder.h"
#include "HsKA_DAB_plus/radio_gui.h"
#include "HsKA_DAB_plus/py_callback.h"
#include "HsKA_DAB_plus/time_deinterleaver.h"
#include "HsKA_DAB_plus/unpuncture_msc.h"
#include "HsKA_DAB_plus/viterbi_wrapper.h"
#include "HsKA_DAB_plus/MSC_Scrambler.h"
#include "HsKA_DAB_plus/firecode_checker.h"
#include "HsKA_DAB_plus/reed_solomon_decoder.h"
#include "HsKA_DAB_plus/MP4_decoder.h"
%}

/* turn on director wrapping Callback */
%feature("director") py_callback;

%include "HsKA_DAB_plus/Super_Frame_Synchronisation.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, Super_Frame_Synchronisation);
%include "HsKA_DAB_plus/OFDM_Symbol_Synchronisation.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, OFDM_Symbol_Synchronisation);
%include "HsKA_DAB_plus/OFDM_coarse_frequency_correction.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, OFDM_coarse_frequency_correction);
%include "HsKA_DAB_plus/DQPSK_Demodulation.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, DQPSK_Demodulation);
%include "HsKA_DAB_plus/Remove_First_OFDM_Symbol.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, Remove_First_OFDM_Symbol);
%include "HsKA_DAB_plus/Frequency_Deinterleaver.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, Frequency_Deinterleaver);
%include "HsKA_DAB_plus/Soft_Demapper.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, Soft_Demapper);
%include "HsKA_DAB_plus/Departitioning.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, Departitioning);
%include "HsKA_DAB_plus/unpuncture_fic.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, unpuncture_fic);

%include "HsKA_DAB_plus/FIC_Scrambler.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, FIC_Scrambler);
%include "HsKA_DAB_plus/CRC16.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, CRC16);
%include "HsKA_DAB_plus/fib_decoder.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, fib_decoder);
%include "HsKA_DAB_plus/radio_gui.h"

%include "HsKA_DAB_plus/py_callback.h"
%include "HsKA_DAB_plus/time_deinterleaver.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, time_deinterleaver);
%include "HsKA_DAB_plus/unpuncture_msc.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, unpuncture_msc);
%include "HsKA_DAB_plus/viterbi_wrapper.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, viterbi_wrapper);
%include "HsKA_DAB_plus/MSC_Scrambler.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, MSC_Scrambler);
%include "HsKA_DAB_plus/firecode_checker.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, firecode_checker);
%include "HsKA_DAB_plus/reed_solomon_decoder.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, reed_solomon_decoder);
%include "HsKA_DAB_plus/MP4_decoder.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, MP4_decoder);
