#include "algocolors.h"
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <QDebug>


/**
 * @brief Default constructor of AlgoColors - Does nothing
 */
AlgoColors::AlgoColors(){}

/**
 * @brief
 * @param thresh
 */
void AlgoColors::morphObs(cv::Mat &thresh){
        cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        //dilate with larger element so make sure object is nicely visible
        cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));

        cv::erode(thresh, thresh, erodeElement);
        cv::erode(thresh, thresh, erodeElement);

        cv::dilate(thresh, thresh, dilateElement);
        cv::dilate(thresh, thresh, dilateElement);
}

/**
 * @brief This method tells if the object is found or not.
 * @param x
 * @param y
 * @param threshold
 * @param cameraFeed
 * @return boolean
 */
bool AlgoColors::trackFilteredObject(int &x, int &y, cv::Mat threshold, cv::Mat &cameraFeed){

    //qDebug() << "Intérieur de la méthode";
    cv::Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    cv::findContours(temp, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if (numObjects<50){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {
                cv::Moments moment = cv::moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if (area>(20*20) && area<((640*480)/1.5) && area>refArea){
                    x = moment.m10 / area;
                    y = moment.m01 / area;
                    objectFound = true;
                    refArea = area;
                }
                else objectFound = false;
            }
            //let user know you found an object
            if (objectFound == true)
                return true ;
            return false;
            //return true ;
        }
        /*else{
            cv::putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", cv::Point(0, 50), 1, 2, cv::Scalar(0, 0, 255), 2);
            return false ;
        }*/
    }

    return false ;
}

