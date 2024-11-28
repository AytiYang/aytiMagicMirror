#include "smile.h"
#include <QDebug>

void cvTest() {
//    // 创建一个视频捕捉对象
//    qDebug() << "cvTest";
//    cv::VideoCapture cap(0);
//    if (!cap.isOpened()) {
//        std::cerr << "无法打开摄像头" << std::endl;
//        return;
//    }

//    // 加载人脸检测的分类器
//    cv::CascadeClassifier face_cascade;
//    if (!face_cascade.load(cv::samples::findFile("haarcascade_frontalface_default.xml"))) {
//        std::cerr << "无法加载人脸检测分类器" << std::endl;
//        return;
//    }

//    cv::Mat frame;
//    while (true) {
//        // 捕捉视频帧
//        cap >> frame;
//        if (frame.empty()) {
//            std::cerr << "捕捉到空帧" << std::endl;
//            break;
//        }

//        // 转换为灰度图像
//        cv::Mat gray;
//        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
//        cv::equalizeHist(gray, gray);

//        // 检测人脸
//        std::vector<cv::Rect> faces;
//        face_cascade.detectMultiScale(gray, faces);

//        // 绘制矩形框
//        for (size_t i = 0; i < faces.size(); i++) {
//            cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 2);
//        }

//        // 显示结果
//        cv::imshow("人脸检测", frame);

//        // 按下 'q' 键退出
//        if (cv::waitKey(10) == 'q') {
//            break;
//        }
//    }
//}
}



using namespace cv;
void cvTest2()
{
    int c = 0;
    for(int i=0;i<10;i++)
    {
        c = i;
        std::cout << c << std::endl;

    }
    VideoCapture cap(0);
    Mat img;
    while (true)
    {
        cap.read(img);
        imshow("Image", img);
    }
}
