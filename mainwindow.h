#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTableWidget>
#include <QDateTime>
#include <QJsonObject>
#include "api/ApiManager.h"


#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateTime();
    void updateWeather();
    void handleWeatherResponse(const QJsonObject &json);
    void handleWeatherResponse2(const QJsonObject &json);

    void updateFrame();
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *apiTimer;
    ApiManager *apiManager;
    ApiManager *apiManager2;

    //QLabel *cameraLabel;
    //QTimer *timer;
    cv::VideoCapture cap;

    QImage matToQImage(const cv::Mat& mat);

    void setTableRowsOpacity(QTableWidget* table);
};

#endif // MAINWINDOW_H
