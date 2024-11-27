// mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    // 设置定时器每分钟触发一次
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(30000); // 60000 毫秒 = 1 分钟
    setTableRowsOpacity(ui->futureTable);
    // 初始化时立即更新一次时间
    updateTime();
}

MainWindow::~MainWindow()
{
    delete ui;
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
