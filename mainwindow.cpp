// mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    // 设置定时器每分钟触发一次
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(60000); // 60000 毫秒 = 1 分钟

    // 初始化时立即更新一次时间
    updateTime();
}

MainWindow::~MainWindow()
{
    delete ui;
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
