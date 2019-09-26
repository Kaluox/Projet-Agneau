#ifndef OBJECTINFORMATION_H
#define OBJECTINFORMATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std ;

class ObjectInformation{
    public:
        ObjectInformation();
        void setContours(vector<vector<cv::Point> >);
        void setCenter(cv::Point2f);
        void setRadius(float);
        vector<vector<cv::Point> > getContours();
        cv::Point2f getCenter();
        float getRadius();
    private:
        vector<vector<cv::Point> > contours;
        cv::Point2f center;
        float radius;
};

#endif // OBJECTINFORMATION_H
