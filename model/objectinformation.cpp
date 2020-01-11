#include "objectinformation.h"

ObjectInformation::ObjectInformation(){}
void ObjectInformation::setContours(std::vector<std::vector<cv::Point> > tmpContours){contours = tmpContours;}
void ObjectInformation::setCenter(cv::Point2f tmpCenter){center = tmpCenter;}
void ObjectInformation::setRadius(float tmpRadius){radius = tmpRadius;}
std::vector<std::vector<cv::Point> > ObjectInformation::getContours(){return contours;}
cv::Point2f ObjectInformation::getCenter(){return center;}
float ObjectInformation::getRadius(){return radius;}
