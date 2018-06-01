#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f(":/style/style.css");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream t(&f);
    a.setStyleSheet(t.readAll());

    MainWindow w;
    w.show();

    return a.exec();
}
