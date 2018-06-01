#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSound>
#include <QDir>
#include <QFontDatabase>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(200, 200);
    setupCentral();
    setupSignalsAndSlots();
    setupLayout();
    setupStyle();
}

void MainWindow::setupCentral()
{
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);

    secLabel = new QLabel("00");
    minLabel = new QLabel("00");
    hourLabel = new QLabel("00");

    secPlus = new QPushButton;
    minPlus = new QPushButton;
    hourPlus = new QPushButton;
    secMines = new QPushButton;
    minMines = new QPushButton;
    hourMines = new QPushButton;

    startButton = new QPushButton("Start", this);
    pauseButton = new QPushButton("Pause", this);
    stopButton = new QPushButton("Stop", this);

    radioGroup = new QGroupBox("Options");
    radioGroup->setCheckable(true);
    timer = new QRadioButton("Timer");
    chronometer = new QRadioButton("Chronometer");
    chronometer->setChecked(true);

    m_timer = new QTimer(this);

    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/audio/alarm.mp3"));
}


void MainWindow::setupSignalsAndSlots()
{
    connect(secPlus, SIGNAL(clicked(bool)), this, SLOT(addSec()));
    connect(minPlus, SIGNAL(clicked(bool)), this, SLOT(addMin()));
    connect(hourPlus, SIGNAL(clicked(bool)), this, SLOT(addHour()));
    connect(secMines, SIGNAL(clicked(bool)), this, SLOT(takeSec()));
    connect(minMines, SIGNAL(clicked(bool)), this, SLOT(takeMin()));
    connect(hourMines, SIGNAL(clicked(bool)), this, SLOT(takeHour()));

    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause()));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(counting()));
    connect(this, SIGNAL(timeout()), this, SLOT(alarm()));
}

void MainWindow::setupLayout()
{
    QVBoxLayout *min = new QVBoxLayout;
    min->addWidget(minPlus);
    min->addWidget(minLabel);
    min->addWidget(minMines);

    QVBoxLayout *sec = new QVBoxLayout;
    sec->addWidget(secPlus);
    sec->addWidget(secLabel);
    sec->addWidget(secMines);

    QVBoxLayout *hour = new QVBoxLayout;
    hour->addWidget(hourPlus);
    hour->addWidget(hourLabel);
    hour->addWidget(hourMines);

    QHBoxLayout *t = new QHBoxLayout;
    t->addLayout(hour);
    t->addLayout(min);
    t->addLayout(sec);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(startButton);
    buttons->addWidget(pauseButton);
    buttons->addWidget(stopButton);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->layout()->addWidget(timer);
    radioLayout->layout()->addWidget(chronometer);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(t);
    mainLayout->addLayout(buttons);
    mainLayout->addLayout(radioLayout);

    mainWidget->setLayout(mainLayout);
}

void MainWindow::setupStyle()
{
    // label style
    QFontDatabase::addApplicationFont(":font/Segment7Standard.otf");
    QFont f("Segment7, Demi Bold Italic", 30);
    secLabel->setFont(f);
    minLabel->setFont(f);
    hourLabel->setFont(f);
    secLabel->setAlignment(Qt::AlignCenter);
    minLabel->setAlignment(Qt::AlignCenter);
    hourLabel->setAlignment(Qt::AlignCenter);
    // buttons
    secPlus->setProperty("plusButton", true);
    minPlus->setProperty("plusButton", true);
    hourPlus->setProperty("plusButton", true);
    secMines->setProperty("minesButton", true);
    minMines->setProperty("minesButton", true);
    hourMines->setProperty("minesButton", true);

    startButton->setProperty("controlButton", true);
    pauseButton->setProperty("controlButton", true);
    stopButton->setProperty("controlButton", true);

    int w = 55;
    secPlus->setFixedWidth(w);
    minPlus->setFixedWidth(w);
    hourPlus->setFixedWidth(w);
    secMines->setFixedWidth(w);
    minMines->setFixedWidth(w);
    hourMines->setFixedWidth(w);
}


void MainWindow::clear()
{
    m_timer->stop();
    secLabel->setText("00");
    minLabel->setText("00");
    hourLabel->setText("00");
    player->stop();
    running = false;
}

void MainWindow::start()
{
    player->stop();
    if(!running){
        running = true;
        counting();
    }
}

void MainWindow::counting()
{
    if(timer->isChecked()){		// timer
        if(secLabel->text().toInt() != 0
                || minLabel->text().toInt() != 0
                || hourLabel->text().toInt() != 0){

            m_timer->start(1000);
            takeSec();
        }
        else{ pause(); }
    }
    else{		// chronometer
        m_timer->start(1000);
        addSec();
    }
}


void MainWindow::pause()
{
    m_timer->stop();
    running = false;
}

void MainWindow::alarm()
{
    player->play();
    running = false;
}

/** ADD AND TAKE TIME **/

void MainWindow::addSec()
{
    int s = secLabel->text().toInt();
    if(s == 59){
        s = 0;
        addMin();
    } else { s++; }

    QString n = QString::number(s);
    if(s < 10) { n.prepend("0"); }
    secLabel->setText(n);
}

void MainWindow::addMin()
{
    int m = minLabel->text().toInt();
    if(m == 59){
        m = 0;
        addHour();
    }
    else { m++; }

    QString n = QString::number(m);
    if(m < 10) { n.prepend("0"); }
    minLabel->setText(n);
}


void MainWindow::addHour()
{
    int h = hourLabel->text().toInt();
    h++;
    QString s = QString::number(h);
    if(h < 10) { s.prepend("0"); }
    hourLabel->setText(s);
}

void MainWindow::takeSec()
{
    int x = secLabel->text().toInt();
    if(x > 0){
        x--;
    }
    else if (x == 0 && minLabel->text().toInt() > 0){
        x = 59;
        takeMin();
    }

    QString s = QString::number(x);
    if(x < 10) { s.prepend("0"); }

    secLabel->setText(s);

    if(secLabel->text().toInt() == 0
            && minLabel->text().toInt() == 0
            && hourLabel->text().toInt() == 0
            && running) {

        emit timeout();
        running = false;
    }
}

void MainWindow::takeMin()
{
    int x = minLabel->text().toInt();
    if(x > 0){
        x--;
    }
    else if (x == 0 && hourLabel->text().toInt() > 0){
        x = 59;
        takeHour();
    }

    QString s = QString::number(x);
    if(x < 10) { s.prepend("0"); }

    minLabel->setText(s);
}

void MainWindow::takeHour()
{
    int x = hourLabel->text().toInt();
    if(x != 0){
        x--;
    }

    QString s = QString::number(x);
    if(x < 10) { s.prepend("0"); }

    hourLabel->setText(s);

}

