#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri Aug 31 19:41:06 2018
##################################################

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt5 import Qt
from PyQt5 import Qt, QtCore
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import HsKA_DAB_plus
import sip
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")

        if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
            self.restoreGeometry(self.settings.value("geometry").toByteArray())
        else:
            self.restoreGeometry(self.settings.value("geometry", type=QtCore.QByteArray))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 2048000

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_const_sink_x_0 = qtgui.const_sink_c(
        	1536, #size
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_const_sink_x_0.set_update_time(0.40)
        self.qtgui_const_sink_x_0.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0.enable_autoscale(True)
        self.qtgui_const_sink_x_0.enable_grid(True)
        self.qtgui_const_sink_x_0.enable_axis_labels(True)

        if not True:
          self.qtgui_const_sink_x_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
                  "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_const_sink_x_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_const_sink_x_0_win)
        self.fft_vxx_0 = fft.fft_vcc(2048, True, (window.rectangular(2048)), True, 1)
        self.blocks_wavfile_source_0 = blocks.wavfile_source('/home/gnuradiouser/gnuradio/HsKA-DAB+/Signal Examples/DAB+_Radio_Signal_218640000', True)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 1536)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_null_sink_0_0_0 = blocks.null_sink(gr.sizeof_char*1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.HsKA_DAB_plus_Super_Frame_Synchronisation_0 = HsKA_DAB_plus.Super_Frame_Synchronisation(2656, 10)
        self.HsKA_DAB_plus_OFDM_coarse_frequency_correction_0 = HsKA_DAB_plus.OFDM_coarse_frequency_correction(2048, 1536, 504)
        self.HsKA_DAB_plus_OFDM_Symbol_Synchronisation_0 = HsKA_DAB_plus.OFDM_Symbol_Synchronisation(2048, 504, 15, 8)
        self.HsKA_DAB_plus_DQPSK_Demodulation_0 = HsKA_DAB_plus.DQPSK_Demodulation(1536, 2048)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.HsKA_DAB_plus_DQPSK_Demodulation_0, 1), (self.blocks_null_sink_0_0_0, 0))
        self.connect((self.HsKA_DAB_plus_DQPSK_Demodulation_0, 0), (self.blocks_vector_to_stream_0, 0))
        self.connect((self.HsKA_DAB_plus_OFDM_Symbol_Synchronisation_0, 1), (self.HsKA_DAB_plus_DQPSK_Demodulation_0, 1))
        self.connect((self.HsKA_DAB_plus_OFDM_Symbol_Synchronisation_0, 2), (self.HsKA_DAB_plus_OFDM_coarse_frequency_correction_0, 1))
        self.connect((self.HsKA_DAB_plus_OFDM_Symbol_Synchronisation_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.HsKA_DAB_plus_OFDM_coarse_frequency_correction_0, 1), (self.HsKA_DAB_plus_DQPSK_Demodulation_0, 2))
        self.connect((self.HsKA_DAB_plus_OFDM_coarse_frequency_correction_0, 0), (self.HsKA_DAB_plus_DQPSK_Demodulation_0, 0))
        self.connect((self.HsKA_DAB_plus_Super_Frame_Synchronisation_0, 0), (self.HsKA_DAB_plus_OFDM_Symbol_Synchronisation_0, 0))
        self.connect((self.HsKA_DAB_plus_Super_Frame_Synchronisation_0, 1), (self.HsKA_DAB_plus_OFDM_Symbol_Synchronisation_0, 1))
        self.connect((self.blocks_float_to_complex_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.HsKA_DAB_plus_Super_Frame_Synchronisation_0, 0))
        self.connect((self.blocks_vector_to_stream_0, 0), (self.qtgui_const_sink_x_0, 0))
        self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_wavfile_source_0, 1), (self.blocks_float_to_complex_0, 1))
        self.connect((self.fft_vxx_0, 0), (self.HsKA_DAB_plus_OFDM_coarse_frequency_correction_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)


def main(top_block_cls=top_block, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
