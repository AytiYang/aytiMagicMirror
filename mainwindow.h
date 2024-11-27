#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTableWidget>
#include <QDateTime>
#include <QJsonObject>
#include "api/ApiManager.h"

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

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *apiTimer;
    ApiManager *apiManager;
    ApiManager *apiManager2;

    void setTableRowsOpacity(QTableWidget* table);
};

#endif // MAINWINDOW_H
