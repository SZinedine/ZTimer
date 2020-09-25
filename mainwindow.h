#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QGroupBox>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMenu>
#include <QPushButton>
#include <QRadioButton>
#include <QSound>
#include <QTime>
#include <QTimer>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    void setupCentral();
    void setupMenu();
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
    void customContextMenuRequested(QPoint pos);
    void setTime(int m);
    void saveSettings();
    void loadSettings();
    void about();

signals:
    void timeout();

private:
    QLabel* secLabel;
    QLabel* minLabel;
    QLabel* hourLabel;
    QPushButton* secPlus;
    QPushButton* minPlus;
    QPushButton* hourPlus;
    QPushButton* secMinus;
    QPushButton* minMinus;
    QPushButton* hourMinus;
    QPushButton* startButton;
    QPushButton* pauseButton;
    QPushButton* stopButton;
    QGroupBox* radioGroup;
    QRadioButton* timer;
    QRadioButton* chronometer;
    QTimer* m_timer;
    QMediaPlayer* player;
    bool running;   // to prevent the alarm when the user just configuring the clock

    // context menu actions
    QMenu* contextMenu;
    QAction* min5Action;
    QAction* min10Action;
    QAction* min30Action;
    QAction* min45Action;
    QAction* startAction;
    QAction* pauseAction;
    QAction* clearAction;
    QAction* aboutAction;
    QAction* quitAction;
};

#endif   // MAINWINDOW_H
