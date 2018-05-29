#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMediaPlayer>
#include <QRadioButton>
#include <QGroupBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void setupCentral();
    void setupLayout();
    void setupSignalsAndSlots();
    void setupStyle();


private slots:
    void addSec();
    void addMin();
    void addHour();
    void takeSec();
    void takeMin();
    void takeHour();
    void clear();
    void start();
    void counting();
    void pause();
    void alarm();

signals:
    void timeout();

private:
    QWidget *mainWidget;
    QLabel *secLabel;
    QLabel *minLabel;
    QLabel *hourLabel;
    QPushButton *secPlus;
    QPushButton *minPlus;
    QPushButton *hourPlus;
    QPushButton *secMines;
    QPushButton *minMines;
    QPushButton *hourMines;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QGroupBox *radioGroup;
    QRadioButton *timer;
    QRadioButton *chronometer;
    QTimer *m_timer;
    QMediaPlayer *player;
    bool running = false;	// to prevent the alarm when the user just configuring the clock
};

#endif // MAINWINDOW_H
