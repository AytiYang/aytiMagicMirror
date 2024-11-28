#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "api/ApiManager.h"






MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , apiManager(new ApiManager(this))
    , apiManager2(new ApiManager(this)) // 初始化 apiManager2
    ,cap(0)
{
    ui->setupUi(this);
    // 设置定时器每分钟触发一次
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(30000); // 30000 毫秒 = 30 秒
    setTableRowsOpacity(ui->futureTable);
    // 初始化时立即更新一次时间
    updateTime();

    // 设置API定时器每15分钟触发一次
    apiTimer = new QTimer(this);
    connect(apiTimer, &QTimer::timeout, this, &MainWindow::updateWeather);
    apiTimer->start(3000); // 900000 毫秒 = 15 分钟

    // 初始化时立即调用一次API
    updateWeather();
       //ui->photoLabel->setFixedSize(1000,1000);
       if (!cap.isOpened()) {
           qWarning("Error: Could not open camera");
           return;
       }

       connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
       timer->start(20); // 每 30 毫秒更新一次画面




}


void MainWindow::setTableRowsOpacity(QTableWidget* table) {
    int rowCount = table->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        int opacity = 255 - i * 51; // 每行减少20%的不透明度 (255 * 0.2 = 51)
        if (opacity < 0) opacity = 0; // 确保不透明度不低于0

        for (int j = 0; j < table->columnCount(); ++j) {
            QTableWidgetItem* item = table->item(i, j);
            if (!item) {
                item = new QTableWidgetItem();
                table->setItem(i, j, item);
            }
            item->setForeground(QColor(255, 255, 255, opacity)); // 设置文本颜色为白色，并调整透明度
        }
    }
}

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    ui->workdayLabel->setText(currentTime.date().toString("ddd"));
    ui->dateLabel->setText(currentTime.date().toString("MMM dd"));
    ui->yearLabel->setText(currentTime.toString("yyyy"));
    ui->timeLabel->setText(currentTime.toString("HH mm"));
    //ui->timeLabel->setText(currentTime.toString("yyyy-MM-dd HH:mm"));
}

void MainWindow::updateWeather()
{
    QString url = "https://restapi.amap.com/v3/weather/weatherInfo";
    QMap<QString, QString> params;
    params.insert("city", "440310");
    params.insert("key", "7bb0a815f083142e2b62fd5cf3aa9c75");

    QMap<QString, QString> params2;
    params2.insert("city", "440311");
    params2.insert("key", "7bb0a815f083142e2b62fd5cf3aa9c75");

    // 断开之前的连接，防止重复连接
    disconnect(apiManager, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherResponse);
    connect(apiManager, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherResponse);
    disconnect(apiManager2, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherResponse2);
    connect(apiManager2, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherResponse2);

    qDebug() << "sendRequest";
    apiManager->sendRequest(url, params);
    apiManager2->sendRequest(url, params2);
}

void MainWindow::handleWeatherResponse(const QJsonObject &json)
{
    qDebug() << "handleWeatherResponse";
    qDebug() << "Received JSON:" << json;
    // 在这里处理JSON数据，例如更新UI
    // 你可以在这里编写你的逻辑
}

void MainWindow::handleWeatherResponse2(const QJsonObject &json)
{
    qDebug() << "handleWeatherResponse2";
    qDebug() << "Received JSON:" << json;
    // 在这里处理JSON数据，例如更新UI
    // 你可以在这里编写你的逻辑
}


void MainWindow::updateFrame()
{
    cv::Mat frame;
    cap >> frame; // 读取摄像头当前帧
    if (frame.empty()) {
        return;
    }

    // 获取QLabel的大小
    QSize labelSize = ui->photoLabel->size();

    // 将图像转换为QImage
    QImage qImage = matToQImage(frame);

    // 调整QImage的大小以适应QLabel的大小
    QImage resizedImage = qImage.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 将调整大小后的QImage转换为QPixmap并显示在QLabel上
    ui->photoLabel->setPixmap(QPixmap::fromImage(resizedImage));
}




QImage MainWindow::matToQImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC1:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    case CV_8UC3:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
    default:
        qWarning("Unsupported image format");
        return QImage();
    }
}


MainWindow::~MainWindow() {
    cap.release(); // 释放摄像头
    delete timer;
    delete apiManager;
    delete apiManager2;
    delete apiTimer;
    delete ui;
}
