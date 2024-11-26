#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include "sql/sqlConnect.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test();
    MainWindow w;
    //w.show();
    w.showFullScreen();
    return a.exec();
}



//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    QMainWindow window;

//    // 创建数据系列
//        QLineSeries *series = new QLineSeries();
//        series->append(QDateTime(QDate(2023, 1, 1), QTime(0, 0)).toMSecsSinceEpoch(), 7);
//        series->append(QDateTime(QDate(2023, 1, 2), QTime(0, 0)).toMSecsSinceEpoch(), 8);
//        series->append(QDateTime(QDate(2023, 1, 3), QTime(0, 0)).toMSecsSinceEpoch(), 7);
//        // 添加更多数据点...

//        // 创建图表
//        QChart *chart = new QChart();
//        chart->legend()->hide();
//        chart->addSeries(series);
//        chart->setTitle("用户1个月内的起床时间");

//        // 创建时间轴
//        QDateTimeAxis *axisX = new QDateTimeAxis;
//        axisX->setFormat("MM-dd");
//        axisX->setTitleText("日期");
//        chart->addAxis(axisX, Qt::AlignBottom);
//        series->attachAxis(axisX);

//        // 创建数值轴
//        QValueAxis *axisY = new QValueAxis;
//        axisY->setLabelFormat("%i");
//        axisY->setTitleText("起床时间（小时）");
//        chart->addAxis(axisY, Qt::AlignLeft);
//        series->attachAxis(axisY);

//        // 创建图表视图
//        QChartView *chartView = new QChartView(chart);
//        chartView->setRenderHint(QPainter::Antialiasing);

//        window.setCentralWidget(chartView);
//        window.resize(800, 600);
//        window.show();

//        return a.exec();
//}

