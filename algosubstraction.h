#ifndef ALGOSUBSTRACTION_H
#define ALGOSUBSTRACTION_H

#include "node.h"
#include "objectinformation.h"
#include "trajectory.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

class AlgoSubstraction{
    public:
        AlgoSubstraction(int shiftMax, cv::Mat& startFrame, cv::Mat* choosenObject, int nbFrame);
        void run(cv::Mat & currentFrame, int nbFrame, int&, int&);
        cv::Mat generateBinaryImage(cv::Mat &);
        void computeHistogram(cv::Mat &);

        Trajectory getTrajectory() ;

    private:
        int shiftMax;     //Parameters of deplacementmax
        cv::Mat startFrame;        //image of start frame
        cv::Mat *choosenObject;        //object choose
        cv::Mat binaryBackground;        //binary background image
        cv::Mat element;
        cv::Point2f centerBack;    //The last value of the array 'center'
        bool firstFrame;
        double biggestSurface; // the surface of the biggest contour
        int biggestSurfaceId;     // the number of the contour which has the biggest surface
        int count_refond;

        double thresholdRedMin;    //The lower limit of red channel
        double thresholdRedMax;    //The upper limit of red channel
        double thresholdGreenMin;  //The lower limit of green channel
        double thresholdGreenMax;  //The upper limit of green channel
        double thresholdBlueMin;   //The lower limit of blue channel
        double thresholdBlueMax;   //The upper limit of blue channel

        ObjectInformation myObject;
        Trajectory myTrajectoire;

};

#endif // ALGOSUBSTRACTION_H
