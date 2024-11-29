#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "api/ApiManager.h"
#include "opencv2/dnn.hpp"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer_30s(new QTimer(this))
    , timer_15min(new QTimer(this))
    , timer_20ms(new QTimer(this))
    , apiManager_weatherRealtime(new ApiManager(this))
    , apiManager_weatherForecast(new ApiManager(this)) // 初始化 apiManager_weatherForecast
    ,cap(0)
{
    ui->setupUi(this);
    //connections
    {
        connect(timer_30s, &QTimer::timeout, this, &MainWindow::updateTime);
        timer_30s->start(30000); // 30000 毫秒 = 30 秒

        connect(timer_15min, &QTimer::timeout, this, &MainWindow::updateWeather);
        timer_15min->start(900000); // 900000 毫秒 = 15 分钟

        connect(apiManager_weatherRealtime, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherRealtimeResponse);
        connect(timer_20ms, &QTimer::timeout, this, &MainWindow::updateFrame);
        timer_20ms->start(10); // 每 20 毫秒更新一次画面

    }
    //set UI
    {
        ui->futureTable->setSelectionMode(QAbstractItemView::NoSelection);
        setTableRowsOpacity(ui->futureTable);
    }
    //init
    {
        // 初始化时立即更新一次时间
        updateTime();
        updateWeather();
    }
    //warnings
    {
        if (!cap.isOpened())
        {
            qWarning("Error: Could not open camera");
            return;
        }
    }
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

void MainWindow::setTableData(QTableWidget* table)
{

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
    //QString url = "https://restapi.amap.com/v3/weather/weatherInfo";
    QString url = "https://api.caiyunapp.com/v2.6/9HViuHwYCS7mRnrv/114.243,22.4226/realtime";
    QMap<QString, QString> params;
    //params.insert("city", "440310");
    //params.insert("key", "7bb0a815f083142e2b62fd5cf3aa9c75");


    //QMap<QString, QString> params2;
    //params2.insert("city", "440311");
    //params2.insert("key", "7bb0a815f083142e2b62fd5cf3aa9c75");

    // 断开之前的连接，防止重复连接
//    disconnect(apiManager_weatherRealtime, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherRealtimeResponse);
//    connect(apiManager_weatherRealtime, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherRealtimeResponse);
    //disconnect(apiManager_weatherForecast, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherForecastResponse);
    //connect(apiManager_weatherForecast, &ApiManager::jsonReceived, this, &MainWindow::handleWeatherForecastResponse);

    qDebug() << "sendRequest";
    apiManager_weatherRealtime->sendRequest(url, params);
    //apiManager_weatherForecast->sendRequest(url, params2);
}

void MainWindow::handleWeatherRealtimeResponse(const QJsonObject &json)
{
    qDebug() << "Received JSON:" << json;

    // 创建一个QMap来存储解析后的数据
    QMap<QString, QString> weatherData;

    // 解析JSON数据
    if (json.contains("result") && json["result"].isObject()) {
        QJsonObject result = json["result"].toObject();

        if (result.contains("realtime") && result["realtime"].isObject()) {
            QJsonObject realtime = result["realtime"].toObject();

            if (realtime.contains("temperature")) {
                weatherData["temperature"] = QString::number(realtime["temperature"].toDouble());
            }

            if (realtime.contains("humidity")) {
                weatherData["humidity"] = QString::number(realtime["humidity"].toDouble());
            }

            if (realtime.contains("wind") && realtime["wind"].isObject()) {
                QJsonObject wind = realtime["wind"].toObject();
                if (wind.contains("speed")) {
                    weatherData["wind_speed"] = QString::number(wind["speed"].toDouble());
                }
                if (wind.contains("direction")) {
                    weatherData["wind_direction"] = QString::number(wind["direction"].toDouble());
                }
            }

            if (realtime.contains("air_quality") && realtime["air_quality"].isObject()) {
                QJsonObject air_quality = realtime["air_quality"].toObject();
                if (air_quality.contains("aqi") && air_quality["aqi"].isObject()) {
                    QJsonObject aqi = air_quality["aqi"].toObject();
                    if (aqi.contains("chn")) {
                        weatherData["aqi_chn"] = QString::number(aqi["chn"].toInt());
                    }
                    if (aqi.contains("usa")) {
                        weatherData["aqi_usa"] = QString::number(aqi["usa"].toInt());
                    }
                }
            }
        }
    }

    // 打印解析后的数据
    qDebug() << "Temperature:" << weatherData["temperature"];
    qDebug() << "Humidity:" << weatherData["humidity"];
    qDebug() << "Wind Speed:" << weatherData["wind_speed"];
    qDebug() << "Wind Direction:" << weatherData["wind_direction"];
    qDebug() << "AQI (CHN):" << weatherData["aqi_chn"];
    qDebug() << "AQI (USA):" << weatherData["aqi_usa"];
    //UI
    ui->tempLabel->setText(((weatherData["temperature"]).split("."))[0]+"°C");
    ui->humidityLabel->setText(((weatherData["humidity"]).split("."))[1]+ "%");
}

void MainWindow::handleWeatherForecastResponse(const QJsonObject &json)
{
    qDebug() << "handleWeatherForecastResponse";
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

    // 缩小图像以加快检测速度
    cv::Mat smallFrame;
    cv::resize(frame, smallFrame, cv::Size(), 0.5, 0.5);

    // 加载深度学习模型
    static cv::dnn::Net net = cv::dnn::readNetFromCaffe("./deploy.prototxt", "./res10_300x300_ssd_iter_140000.caffemodel");

    // 准备输入blob
    cv::Mat blob = cv::dnn::blobFromImage(smallFrame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);

    // 设置输入blob
    net.setInput(blob);

    // 进行前向传播，获取检测结果
    cv::Mat detections = net.forward();

    // 解析检测结果
    std::vector<cv::Rect> faces;
    float* data = (float*)detections.data;
    for (size_t i = 0; i < detections.total(); i += 7) {
        float confidence = data[i + 2];
        if (confidence > 0.5) {
            int x1 = static_cast<int>(data[i + 3] * smallFrame.cols) * 2;
            int y1 = static_cast<int>(data[i + 4] * smallFrame.rows) * 2;
            int x2 = static_cast<int>(data[i + 5] * smallFrame.cols) * 2;
            int y2 = static_cast<int>(data[i + 6] * smallFrame.rows) * 2;
            faces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
        }
    }

    // 加载笑脸检测的分类器
    static cv::CascadeClassifier smileCascade;
    if (smileCascade.empty()) {
        smileCascade.load("./haarcascade_smile.xml");
    }

    // 在检测到的人脸区域内检测笑脸
    for (const auto& face : faces) {
        cv::Mat faceROI = frame(face);
        std::vector<cv::Rect> smiles;
        smileCascade.detectMultiScale(faceROI, smiles, 1.8, 20, 0, cv::Size(25, 25));

        // 在检测到的笑脸周围画矩形框
        for (const auto& smile : smiles) {
            cv::Rect smileRect(face.x + smile.x, face.y + smile.y, smile.width, smile.height);
            cv::rectangle(frame, smileRect, cv::Scalar(0, 255, 255), 4);
        }
    }

    // 在检测到的人脸周围画矩形框
    for (const auto& face : faces) {
        cv::rectangle(frame, face, cv::Scalar(39, 255, 0), 4);
    }

    // 获取QLabel的大小
    QSize labelSize = ui->photoLabel->size();

    // 将处理后的图像转换为QImage
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
    delete timer_20ms;
    delete timer_15min;
    delete timer_30s;
    delete apiManager_weatherRealtime;
    delete apiManager_weatherForecast;
    delete ui;
}
