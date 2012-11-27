TEMPLATE = app
TARGET = widgettest

CONFIG += mobility
MOBILITY += multimedia

INCLUDEPATH += ../fftreal
INCLUDEPATH += ../widget

# Both are needed to use spectrum analyzer
LIBS += -L../fftreal -lfftreal
LIBS += -L../widget -lspectrum

HEADERS = mainwin.h
SOURCES = mainwin.cpp main.cpp

