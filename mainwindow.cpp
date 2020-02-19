#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setFixedSize(200, 200);
    setupCentral();
    setupMenu();
    setupSignalsAndSlots();
    setupLayout();
    setupStyle();
    loadSettings();
}

void MainWindow::setupCentral()
{
    secLabel = new QLabel("00", this);
    minLabel = new QLabel("00", this);
    hourLabel = new QLabel("00", this);

    secPlus = new QPushButton(this);
    minPlus = new QPushButton(this);
    hourPlus = new QPushButton(this);
    secMinus = new QPushButton(this);
    minMinus = new QPushButton(this);
    hourMinus = new QPushButton(this);

    startButton = new QPushButton("Start", this);
    pauseButton = new QPushButton("Pause", this);
    stopButton = new QPushButton("Stop", this);

    radioGroup = new QGroupBox("Options", this);
    radioGroup->setCheckable(true);
    timer = new QRadioButton("Timer", this);
    chronometer = new QRadioButton("Chronometer", this);
    chronometer->setChecked(true);

    m_timer = new QTimer(this);

    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/audio/alarm.mp3"));
}

void MainWindow::setupMenu()
{
    // Context Menu
    contextMenu = new QMenu("context menu", this);
    auto timeMenu = new QMenu("set time", this);
    contextMenu->addMenu(timeMenu);

    min5Action = new QAction("5 minutes", this);
    min10Action = new QAction("10 minutes", this);
    min30Action = new QAction("30 minutes", this);
    min45Action = new QAction("45 minutes", this);

    // control actions
    startAction = new QAction("Start", this);
    pauseAction = new QAction("Pause", this);
    clearAction = new QAction("Reset", this);
    aboutAction = new QAction("About", this);
    quitAction = new QAction("Quit", this);

    timeMenu->addActions({min5Action, min10Action, min30Action, min45Action});
    contextMenu->addActions({startAction, pauseAction, clearAction, aboutAction, quitAction});
}

void MainWindow::setupSignalsAndSlots()
{
    // add sec/min/hr buttons
    connect(secPlus, SIGNAL(clicked(bool)), this, SLOT(addSec()));
    connect(minPlus, SIGNAL(clicked(bool)), this, SLOT(addMin()));
    connect(hourPlus, SIGNAL(clicked(bool)), this, SLOT(addHour()));
    connect(secMinus, SIGNAL(clicked(bool)), this, SLOT(takeSec()));
    connect(minMinus, SIGNAL(clicked(bool)), this, SLOT(takeMin()));
    connect(hourMinus, SIGNAL(clicked(bool)), this, SLOT(takeHour()));
    // control buttons
    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause()));
    // timer
    connect(m_timer, SIGNAL(timeout()), this, SLOT(counting()));
    connect(this, SIGNAL(timeout()), this, SLOT(alarm()));
    // radio buttons that are saved everytime they are changed
    connect(timer, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    connect(chronometer, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    // context menu
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(min5Action,  &QAction::triggered, [=](){ setTime(5);  } );
    connect(min10Action, &QAction::triggered, [=](){ setTime(10); } );
    connect(min30Action, &QAction::triggered, [=](){ setTime(30); } );
    connect(min45Action, &QAction::triggered, [=](){ setTime(45); } );
    connect(startAction, SIGNAL(triggered(bool)), this, SLOT(start()));
    connect(pauseAction, SIGNAL(triggered(bool)), this, SLOT(pause()));
    connect(clearAction, SIGNAL(triggered(bool)), this, SLOT(clear()));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    connect(quitAction,  SIGNAL(triggered(bool)), this, SLOT(close()));
}

void MainWindow::setupLayout()
{
    QVBoxLayout *min = new QVBoxLayout;
    min->addWidget(minPlus);
    min->addWidget(minLabel);
    min->addWidget(minMinus);

    QVBoxLayout *sec = new QVBoxLayout;
    sec->addWidget(secPlus);
    sec->addWidget(secLabel);
    sec->addWidget(secMinus);

    QVBoxLayout *hour = new QVBoxLayout;
    hour->addWidget(hourPlus);
    hour->addWidget(hourLabel);
    hour->addWidget(hourMinus);

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

    auto mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    mainWidget->setLayout(mainLayout);
}

void MainWindow::setupStyle()
{
    // label style
    QFontDatabase::addApplicationFont(":font/Segment7Standard.otf");
    QFont f("Segment7, Demi Bold Italic", 30);
    for (auto& i : {secLabel, minLabel, hourLabel}) {
        i->setFont(f);
        i->setAlignment(Qt::AlignCenter);
    }
    int width = 55;
    for (auto& i : {secPlus, minPlus, hourPlus}) {
        i->setProperty("plusButton", true);
        i->setFixedWidth(width);
    }
    for (auto& i : {secMinus, minMinus, hourMinus}) {
        i->setProperty("minusButton", true);
        i->setFixedWidth(width);
    }
    for (auto& i : {startButton, pauseButton, stopButton})
        i->setProperty("controlButton", true);
}


void MainWindow::clear()
{
    m_timer->stop();
    secLabel->setText("00");
    minLabel->setText("00");
    hourLabel->setText("00");
    player->stop();
}

void MainWindow::start()
{
    running = true;
    counting();
}

void MainWindow::counting()
{
    if(timer->isChecked()) {		// timer
        if(secLabel->text().toInt() != 0
                || minLabel->text().toInt() != 0
                || hourLabel->text().toInt() != 0){

            m_timer->start(1000);
            takeSec();
        }
        else { pause(); }
    }
    else {		// chronometer
        m_timer->start(1000);
        addSec();
    }
}


void MainWindow::pause()
{
    m_timer->stop();
}

void MainWindow::alarm()
{
    player->play();
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
    if(x > 0)
        x--;
    else if (x == 0 && minLabel->text().toInt() == 0 && hourLabel->text().toInt() == 0){
    }
    else if (x == 0 && minLabel->text().toInt() >= 0){
        x = 59;
        takeMin();
    }

    QString s = QString::number(x);
    if(x < 10) s.prepend("0");

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
    if(x > 0)
        x--;
    else if (x == 0 && hourLabel->text().toInt() > 0){
        x = 59;
        takeHour();
    }

    QString s = QString::number(x);
    if(x < 10) s.prepend("0");

    minLabel->setText(s);
}

void MainWindow::takeHour()
{
    int x = hourLabel->text().toInt();
    if(x != 0)
        x--;

    QString s = QString::number(x);
    if(x < 10) s.prepend("0");

    hourLabel->setText(s);
}

void MainWindow::setTime(int m)
{
    if (running) return;
    clear();
    for (int i = 0 ; i < m ; i++) addMin();
}


void MainWindow::customContextMenuRequested(QPoint pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("main");
    int mode = (timer->isChecked()) ? 1 : 2;	// timer = 1 ; stopwatch = 2
    settings.setValue("mode", QVariant(mode));
    settings.endGroup();
}


void MainWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("main");
    int mode = settings.value("mode").toInt();
    if (mode == 1) timer->setChecked(true);
    else chronometer->setChecked(true);
    settings.endGroup();
}


void MainWindow::about()
{
    QString title = QString("About " + qApp->applicationDisplayName());
    QString text = qApp->applicationDisplayName();
    text.prepend("<b>Application: </b>").append("<br>");
    text.append("<b>Version: </b>" + qApp->applicationVersion() + "<br>");
    text.append("<b>Author: </b> Zineddine SAIBI<br>");
    text.append("<b>E-mail: </b> saibi.zineddine@yahoo.com<br><br>");
    text.append("This program is free software: you can redistribute it and/or modify"
                "it under the terms of the GNU General Public License as published by"
                "the Free Software Foundation, either version 3 of the License, or"
                "(at your option) any later version.<br><br>");
    text.append("This program is distributed in the hope that it will be useful, "
                "but WITHOUT ANY WARRANTY; without even the implied warranty of"
                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the"
                "GNU General Public License for more details.<br>");

    QMessageBox::about(this, title, text);
}


