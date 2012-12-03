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
    btn = new QPushButton(REC_START);

    layout->addWidget(analyzer);
    layout->addWidget(btn);
    this->setLayout(layout);

    // Size for our window
    this->resize(480, 320);
}

void MainView::connect_signals()
{
    connect(btn, SIGNAL(clicked()), this, SLOT(btn_cb(void)));
}

/* Public signal methods (slots) */
void MainView::btn_cb()
{
    // Flip state
    recording = !recording;

    if (!recording)
        btn->setText(REC_START);
    else
        btn->setText(REC_STOP);


    if (recording)
        analyzer->startRecording();
    else
        analyzer->stopRecording();

    // Dump measured levels afterwards
    if (!recording) {
        qreal _min, _max;
        analyzer->getLevels(_min, _max);

        qDebug("Minimum audio level: %.2f; Maximum audio level: %.2f",
            _min, _max);
    }
}

