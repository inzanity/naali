#include "LevelAutoDetectDialog.h"

namespace MumbleAudio
{
    LevelAutoDetectDialog::LevelAutoDetectDialog()
    {
        recording = false;
        analyzer = new MumbleAudio::AudioLevelWidget(this);

        create_layout();
        connect_signals();

        // 30 frequency bars
        // range: 200Hz ... 4200Hz
        analyzer->setSpectrumConfig(30, 200, 4200);
    }

    LevelAutoDetectDialog::~LevelAutoDetectDialog()
    {
        delete analyzer;
    }

    void LevelAutoDetectDialog::create_layout()
    {
        layout = new QVBoxLayout();
        silence_btn = new QPushButton(RECORD_SILENCE);
        speech_btn = new QPushButton(RECORD_VOICE);
        done_btn = new QPushButton("Exit");

        // Disable other buttons
        speech_btn->setEnabled(false);
        done_btn->setEnabled(false);

        layout->addWidget(analyzer);
        layout->addWidget(silence_btn);
        layout->addWidget(speech_btn);
        layout->addWidget(done_btn);
        this->setLayout(layout);

        this->resize(320, 400);
    }

    void LevelAutoDetectDialog::connect_signals()
    {
        connect(silence_btn, SIGNAL(clicked()), this, SLOT(silence_levels(void)));
        connect(speech_btn, SIGNAL(clicked()), this, SLOT(speech_levels(void)));
        connect(done_btn, SIGNAL(clicked()), this, SLOT(levels_done(void)));
    }

    /* Event handlers (slots) */

    // Silence levels button
    void LevelAutoDetectDialog::silence_levels()
    {
        // Flip recording state
        recording = !recording;

        if (!recording)
            silence_btn->setText(RECORD_SILENCE);
        else
            silence_btn->setText(STOP_RECORD);

        if (recording)
            analyzer->startRecording(false);
        else {
            analyzer->stopRecording();
            speech_btn->setEnabled(true);
            silence_btn->setEnabled(false);
        }
    }

    // Voice levels button
    void LevelAutoDetectDialog::speech_levels()
    {
        // Flip recording state
        recording = !recording;

        if (!recording)
            speech_btn->setText(RECORD_VOICE);
        else
            speech_btn->setText(STOP_RECORD);

        if (recording)
            analyzer->startRecording(true);
        else {
            analyzer->stopRecording();
            done_btn->setEnabled(true);
            speech_btn->setEnabled(false);
        }
    }

    // Transmit measured levels, close window
    void LevelAutoDetectDialog::levels_done()
    {
        qreal silenceClipLevel, voiceClipLevel;
        qreal low, high;
        analyzer->getLevels(low, high);

        // Scale the levels to [0.0, 1.0] range
        // NOTE: we know the incoming range is [-96.0, 0.0],
        // but we do a "MAX 1.0" in any case. We've seen values up to
        // 1.03 in some cases.
        //
        // XXX: What the...? These are moronic steps, but when building
        // on Windows, they are necessary. A direct call of
        //      qMin(abs(low)/96.0f, 1.0f);
        // causes a compiler error. Namely this:
        // "error C2782: 'const T &qMin(const T &, const T&)': template
        // parameter 'T' is ambiguous"
        //
        // XXX: Why is this ever necessary?
        const qreal low_scaled = abs(low)/96.0f;
        const qreal high_scaled = abs(high)/96.0f;
        const qreal _one = 1.0f;

        silenceClipLevel = qMin(low_scaled, _one);
        voiceClipLevel = qMin(high_scaled, _one);

        // FIXME: emit levels via signal, catch in wizard
        emit ClipLevelsMeasured(silenceClipLevel, voiceClipLevel);

        // Terminate modal dialog
        this->done(1);
    }
}

