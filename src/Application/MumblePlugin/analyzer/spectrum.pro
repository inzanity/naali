include(spectrum.pri)

TEMPLATE = subdirs

# Ensure that library is built before application
CONFIG  += ordered

# FFT library
SUBDIRS += fftreal

# Actual widget
SUBDIRS += widget

TARGET = spectrum

sources.files = spectrum.pri spectrum.pro
sources.path = FIX_THIS_DUMMY_PATH/spectrum
INSTALLS += sources

