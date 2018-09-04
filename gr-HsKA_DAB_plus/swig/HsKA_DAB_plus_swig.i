/* -*- c++ -*- */

#define HSKA_DAB_PLUS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "HsKA_DAB_plus_swig_doc.i"

%{
#include "HsKA_DAB_plus/Super_Frame_Synchronisation.h"
#include "HsKA_DAB_plus/OFDM_Symbol_Synchronisation.h"
#include "HsKA_DAB_plus/OFDM_coarse_frequency_correction.h"
#include "HsKA_DAB_plus/DQPSK_Demodulation.h"
#include "HsKA_DAB_plus/Remove_First_OFDM_Symbol.h"
#include "HsKA_DAB_plus/Frequency_Deinterleaver.h"
#include "HsKA_DAB_plus/Soft_Demapper.h"
%}


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
