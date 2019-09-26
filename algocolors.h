#ifndef ALGOCOLORS_H
#define ALGOCOLORS_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

class AlgoColors{
    public:
        AlgoColors();
        void morphObs(cv::Mat &thresh);
        bool trackFilteredObject(int &x, int &y, cv::Mat threshold, cv::Mat &cameraFeed);
};

#endif // ALGOCOLORS_H
