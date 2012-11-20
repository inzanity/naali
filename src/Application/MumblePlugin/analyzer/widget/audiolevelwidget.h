/*************************************************
 *
 * Copyright (C) 2012 Nomovok Ltd.
 * Contact: http://www.nomovok.com
 *
 * Written for realXtend naali. Based on Qt 4.x demo application
 * "Spectrum Analyzer", stripped down to only provide a frequency bar
 * array.
 *
 * LICENSE: BSD
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 *     of its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 *
 ***********************************************/


#ifndef AUDIOLEVELWIDGET_H
#define AUDIOLEVELWIDGET_H

#include <QWidget>
#include <QtMultimediaKit/QAudio>
#include <QtMultimediaKit/QAudioFormat>

/* Yay, forward declarations */
class Engine;
class Spectrograph;
class LevelMeter;
class FrequencySpectrum;

namespace MumbleAudio
{


/**
 * Spectrum analyzer container widget.
 */
class AudioLevelWidget : public QWidget {
    Q_OBJECT
public:
    AudioLevelWidget(QWidget *parent = 0);
    ~AudioLevelWidget();

    // Limits
    void setSpectrumConfig(int nBars, qreal floorFreq, qreal ceilFreq);

    // QObject
    void timerEvent(QTimerEvent *event);

    // Results
    int getLevels(qint64 *silenceClip, qint64 *speechClip);

    // Just passing through...
    void startRecording(void);
    void stopRecording(void);

public slots:
    void stateChanged(QAudio::Mode mode, QAudio::State state);
    void formatChanged(const QAudioFormat &format);
    void spectrumChanged(qint64 position, qint64 length,
            const FrequencySpectrum &spectrum);

// This probably needs to send out a signal when audio levels have been
// measured...


private slots:
    void initRecord();

private:
    Engine*         m_engine;
    Spectrograph*   m_spectrograph;
    LevelMeter*     m_levelMeter;

    // Audio clip levels
    qint64          m_silenceClipLevel;
    qint64          m_speechClipLevel;

    void reset(void);

    // Internal helpers for debugging
    void infoMessage(const QString &message);

};

} // namespace MumbleAudio

#endif // AUDIOLEVELWIDGET_H
