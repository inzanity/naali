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

#include "audiolevelwidget.h"
#include "engine.h"
#include "spectrograph.h"
#include "levelmeter.h"

#include <QTimerEvent>
#include <QDebug>

namespace MumbleAudio
{

AudioLevelWidget::AudioLevelWidget(QWidget *parent)
    :   QWidget(parent),
    m_engine(new Engine(this)),
    m_spectrograph(new Spectrograph(this)),
    m_levelMeter(new LevelMeter(this))
{
    m_silenceClipLevel = -1;
    m_speechClipLevel = -1;
}

AudioLevelWidget::~AudioLevelWidget()
{
    // Do nothing
}

//---------------------------------------------------------
// Public slots
//---------------------------------------------------------

void AudioLevelWidget::stateChanged(QAudio::Mode mode, QAudio::State state)
{
    Q_UNUSED(mode);

    // Note: we do *not* reset the engine
    if (state != QAudio::ActiveState && state != QAudio::SuspendedState) {
        m_levelMeter->reset();
        m_spectrograph->reset();
    }
}

// Basically a debug routine
void AudioLevelWidget::formatChanged(const QAudioFormat &format)
{
    infoMessage(formatToString(format));
}

// FrequencySpectrum comes via spectrograph.h
void AudioLevelWidget::spectrumChanged(qint64 position, qint64 length,
        const FrequencySpectrum &spectrum)
{
    // These are used in original demo for progress bar updates, but we
    // don't have one here. Mark them unused so compiler won't complain.
    Q_UNUSED(position);
    Q_UNUSED(length);

    m_spectrograph->spectrumChanged(spectrum);
}


//---------------------------------------------------------
// Public methods
//---------------------------------------------------------

// Sets clip level values to provided arguments, returns 0 on success.
// If either of the levels is missing, does nothing to the arguments and
// returns -1.
int AudioLevelWidget::getLevels(qint64 *silenceClip, qint64 *speechClip)
{
    if (m_silenceClipLevel < 0 || m_speechClipLevel < 0) {
        return -1;  // Should probably be a defined error code...
    }

    *silenceClip = m_silenceClipLevel;
    *speechClip = m_speechClipLevel;
    return 0;
}

// Number of spectrograph bars and frequency range,
// passed directly to Spectrograph instance.
void AudioLevelWidget::setSpectrumConfig(int nBars, qreal floorFreq, qreal ceilFreq)
{
    m_spectrograph->setParams(nBars, floorFreq, ceilFreq);
    reset();
}

void AudioLevelWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    qDebug("Timer event fired.");
}

void AudioLevelWidget::startRecording()
{
    m_engine->startRecording();
}

void AudioLevelWidget::stopRecording()
{
    m_engine->stopRecording();
}


//---------------------------------------------------------
// Private slot(s)
//---------------------------------------------------------

void AudioLevelWidget::initRecord()
{
    reset();
    m_engine->initializeRecord();
}



//---------------------------------------------------------
// Private methods
//---------------------------------------------------------

void AudioLevelWidget::reset()
{
    m_engine->reset();
    m_spectrograph->reset();
    m_levelMeter->reset();
}

void AudioLevelWidget::infoMessage(const QString &message)
{
    qDebug() << message;
}

} // namespace MumbleAudio
