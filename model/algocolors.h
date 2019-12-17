#ifndef ALGOCOLORS_H
#define ALGOCOLORS_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

class AlgoColors{
    public:

        /**
         * @brief Default constructor of AlgoColors - Does nothing
         */
        AlgoColors();

        /**
         * @brief The object given is eroded and dilated to be more visible
         * @param cv::Mat & thresh
         */
        void morphObs(cv::Mat &thresh);

        /**
         * @brief This method tells if the object is found or not.
         * @param x
         * @param y
         * @param threshold
         * @param cameraFeed
         * @return boolean
         */
        bool trackFilteredObject(int &x, int &y, cv::Mat threshold, cv::Mat &cameraFeed);
};

#endif // ALGOCOLORS_H
