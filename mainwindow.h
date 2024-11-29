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

#include <QJsonArray>
#include <QMap>
#include <QString>




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
    void handleWeatherRealtimeResponse(const QJsonObject &json);
    void handleWeatherForecastResponse(const QJsonObject &json);

    void updateFrame();
private:
    Ui::MainWindow *ui;
    QTimer *timer_30s;
    QTimer *timer_15min;
    QTimer *timer_20ms;
    ApiManager *apiManager_weatherRealtime;
    ApiManager *apiManager_weatherForecast;

    //QLabel *cameraLabel;
    //QTimer *timer;
    cv::VideoCapture cap;

    QImage matToQImage(const cv::Mat& mat);

    void setTableRowsOpacity(QTableWidget* table);
    void setTableData(QTableWidget* table);
};

#endif // MAINWINDOW_H
