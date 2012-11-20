include(../spectrum.pri)

static: error(This application cannot be statically linked to the fftreal library)

TEMPLATE = lib

TARGET = spectrum

# QtMultimedia? Sorry, missing from debian
CONFIG      += mobility
MOBILITY    += multimedia

SOURCES  += \
            audiolevelwidget.cpp \
            engine.cpp \
            frequencyspectrum.cpp \
            levelmeter.cpp \
            spectrograph.cpp \
            spectrumanalyser.cpp \
            utils.cpp

HEADERS  += \
            audiolevelwidget.h \
            engine.h \
            frequencyspectrum.h \
            levelmeter.h \
            spectrograph.h \
            spectrum.h \
            spectrumanalyser.h \
            utils.h

fftreal_dir = ../fftreal

INCLUDEPATH += $${fftreal_dir}

# Dynamic linkage against FFTReal DLL
macx {
    # Link to fftreal framework
    LIBS += -F$${fftreal_dir}
    LIBS += -framework fftreal
} else {
    LIBS += -L$${fftreal_dir}
    LIBS += -lfftreal
}

# Install


# Deployment

macx {
    # Relocate fftreal.framework into spectrum.app bundle
    framework_dir = ../spectrum.app/Contents/Frameworks
    framework_name = fftreal.framework/Versions/1/fftreal
    QMAKE_POST_LINK = \
        mkdir -p $${framework_dir} &&\
        rm -rf $${framework_dir}/fftreal.framework &&\
        cp -R $${fftreal_dir}/fftreal.framework $${framework_dir} &&\
        install_name_tool -id @executable_path/../Frameworks/$${framework_name} \
                            $${framework_dir}/$${framework_name} &&\
        install_name_tool -change $${framework_name} \
                            @executable_path/../Frameworks/$${framework_name} \
                            ../spectrum.app/Contents/MacOS/spectrum
} else {
    linux-g++*: {
        # Provide relative path from application to fftreal library
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
    }
}


