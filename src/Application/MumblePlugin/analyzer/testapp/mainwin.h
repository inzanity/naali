#ifndef MAINWIN_H_
#define MAINWIN_H_

#include <QtGui>
#include "audiolevelwidget.h"

#define REC_START "Start recording"
#define REC_STOP "Stop recording"

class MainView : public QDialog
{
    Q_OBJECT

public:
    MainView();
    ~MainView();

public slots:
    void btn_cb();

private:
    void create_layout();
    void connect_signals();


    // GUI stuff
    QVBoxLayout     *layout;    // vbox

    QPushButton     *btn;

    // Spectrum analyzer
    MumbleAudio::AudioLevelWidget    *analyzer;

    // Recording button toggle
    bool            recording;

};

#endif // MAINWIN_H_

