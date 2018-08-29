#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/lib
export PATH=/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/lib:$PATH
export LD_LIBRARY_PATH=/home/gnuradiouser/gnuradio/HsKA-DAB+/gr-HsKA_DAB_plus/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-HsKA_DAB_plus 
