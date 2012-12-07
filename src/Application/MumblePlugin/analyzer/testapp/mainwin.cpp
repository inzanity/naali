#include <QDebug>
#include "mainwin.h"
#include "audiolevelwidget.h"

MainView::MainView()
{
    recording = false;
    analyzer = new MumbleAudio::AudioLevelWidget(this);

    create_layout();
    connect_signals();

    analyzer->setSpectrumConfig(30, 80, 4200);
}

MainView::~MainView()
{
    delete analyzer;
}


// Visual stuff

void MainView::create_layout()
{
    layout = new QVBoxLayout();
    btn1 = new QPushButton(REC_START_SILENCE);
    btn2 = new QPushButton(REC_START_VOICE);
    btnX = new QPushButton("Get measured levels");

    layout->addWidget(analyzer);
    layout->addWidget(btn1);
    layout->addWidget(btn2);
    layout->addWidget(btnX);
    this->setLayout(layout);

    // Size for our window
    this->resize(480, 320);
}

void MainView::connect_signals()
{
    connect(btn1, SIGNAL(clicked()), this, SLOT(btn1_cb(void)));
    connect(btn2, SIGNAL(clicked()), this, SLOT(btn2_cb(void)));
    connect(btnX, SIGNAL(clicked()), this, SLOT(btnX_cb(void)));
}

/* Public signal methods (slots) */

// Silence level measuring button
void MainView::btn1_cb()
{
    // Flip state
    recording = !recording;

    if (!recording)
        btn1->setText(REC_START_SILENCE);
    else
        btn1->setText(REC_STOP);


    if (recording)
        analyzer->startRecording(false);
    else
        analyzer->stopRecording();
}

// Voice level measuring button
void MainView::btn2_cb()
{
    // Flip state
    recording = !recording;

    if (!recording)
        btn2->setText(REC_START_VOICE);
    else
        btn2->setText(REC_STOP);


    if (recording)
        analyzer->startRecording(true);
    else
        analyzer->stopRecording();

}

void MainView::btnX_cb()
{
    qreal _clipS, _clipV;
    analyzer->getLevels(_clipS, _clipV);

    qDebug("Silence clip level: %.2f; Voice clip level: %.2f",
        _clipS, _clipV);
}


