#ifndef NODE_H
#define NODE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDateTime>

//using namespace cv;

class Nodes{
    public:
        Nodes();
        Nodes(cv::Point2f center, QDateTime timeNow, int tmpCurrentFrame);
        cv::Point2f &getCenter();
        QString nodeToString();
    private :
        cv::Point2f center;
        QDateTime timeNow;
        int tmpCurrentFrame;
};

#endif // NODE_H
