#ifndef MAINWIN_H_
#define MAINWIN_H_

#include <QtGui>
#include "audiolevelwidget.h"

#define REC_START_SILENCE "Start recording (silence)"
#define REC_START_VOICE "Start recording (voice)"
#define REC_STOP "Stop recording"

class MainView : public QDialog
{
    Q_OBJECT

public:
    MainView();
    ~MainView();

public slots:
    void btn1_cb();
    void btn2_cb();
    void btnX_cb();

private:
    void create_layout();
    void connect_signals();


    // GUI stuff
    QVBoxLayout     *layout;    // vbox

    QPushButton     *btn1;
    QPushButton     *btn2;
    QPushButton     *btnX;

    // Spectrum analyzer
    MumbleAudio::AudioLevelWidget    *analyzer;

    // Recording button toggle
    bool            recording;

};

#endif // MAINWIN_H_

