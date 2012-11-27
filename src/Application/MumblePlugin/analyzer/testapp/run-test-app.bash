#!/bin/bash

_me=$(readlink -f $0)
_mydir=$(dirname ${_me})
_app=${_mydir}/widgettest

FFT_LIB_PATH=${_mydir}/../fftreal
WIDGET_PATH=${_mydir}/../widget

if [ ! -x ${_app} ]; then
    echo Test app not built yet
    exit 1
fi

LD_LIBRARY_PATH=${FFT_LIB_PATH}:${WIDGET_PATH} ${_app}
