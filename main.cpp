#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include "sql/sqlConnect.h"

#include <opencv2/opencv.hpp>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    w.showFullScreen();
    return a.exec();
}


