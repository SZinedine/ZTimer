#include "mainwindow.h"
#include <QApplication>
#include <QFile>

#define ZTIMER_VERSION "0.2"
#define ZTIMER_NAME "ZTimer"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(ZTIMER_NAME);
    a.setApplicationDisplayName(ZTIMER_NAME);
    a.setApplicationVersion(ZTIMER_VERSION);

    QFile f(":/style/style.css");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream t(&f);
    a.setStyleSheet(t.readAll());

    MainWindow w;
    w.show();

    return a.exec();
}
