#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "node.h"


using namespace std ;

class Trajectory{
    public:
        Trajectory();
        void addPoint(Nodes& node) ;
        void drawTrajectoire(cv::Mat& frame);
        vector<Nodes>& getCenterList();
        void setLastCenter(Nodes& node);
        Nodes getLastCenter();
    private:
        vector<Nodes> centerList;
        Nodes centerBack;  //The last value of the array 'center'
};

#endif // TRAJECTORY_H
