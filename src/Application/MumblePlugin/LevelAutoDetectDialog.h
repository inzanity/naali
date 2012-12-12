
#pragma once

#include <QtGui>
#include "audiolevelwidget.h"

#define RECORD_SILENCE "Start measuring (silence)"
#define RECORD_VOICE "Start measuring (voice)"
#define STOP_RECORD "Stop recording"

namespace MumbleAudio
{
    class LevelAutoDetectDialog : public QDialog
    {
        Q_OBJECT

    public:
        LevelAutoDetectDialog();
        ~LevelAutoDetectDialog();

    signals:
        void ClipLevelsMeasured(qreal silenceClip, qreal voiceClip);

    public slots:
        void silence_levels();
        void speech_levels();
        void levels_done();

    private:
        void create_layout();
        void connect_signals();

        QVBoxLayout     *layout;    // vbox
        QPushButton     *silence_btn;
        QPushButton     *speech_btn;
        QPushButton     *done_btn;

        // Analyser - magic thing
        MumbleAudio::AudioLevelWidget   *analyzer;

        // Toggle state
        bool            recording;
    };
}
