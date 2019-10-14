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

        /**
        * @brief Constructor
        * @param shiftMax
        * @param startFrame
        * @param choosenObject
        * @param nbFrame
        */
        AlgoSubstraction(int shiftMax, cv::Mat& startFrame, cv::Mat* choosenObject, int nbFrame);

        /**
        * @brief AlgoSoustraction::decter - La fonction principale de détection et suivie
        * @param currentFrame: Image courante de la vidéo
        * @param nbFrame: Numéro du frame courante
        */
        void run(cv::Mat & currentFrame, int nbFrame, int&, int&);

        /**
         *
         * @brief Get the binary image
         * @param tmp
         * @return cv::Mat
         */
        cv::Mat generateBinaryImage(cv::Mat & tmp);

        /**
         * Histogram
         * @brief AlgoSoustraction::calculeHistogram
         * @param src
         */
        void computeHistogram(cv::Mat & src);

        /**
         * @brief getter of trajectory
         * @return Trajectory
         */
        Trajectory getTrajectory() ;

    private:
        int shiftMax;     //Parameters of deplacement max
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
        Trajectory myTrajectory;

};

#endif // ALGOSUBSTRACTION_H
