#include <QApplication>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("Segoe UI", 10);
    a.setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}
