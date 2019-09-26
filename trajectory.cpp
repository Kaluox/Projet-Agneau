#include "trajectory.h"

Trajectory::Trajectory(){}

void Trajectory::addPoint(Nodes &node){ centerList.push_back(node);}
vector<Nodes>& Trajectory::getCenterList(){ return centerList ; }
void Trajectory::setLastCenter(Nodes &node){ centerBack = node ; }
Nodes Trajectory::getLastCenter(){ return centerBack ; }

void Trajectory::drawTrajectoire(cv::Mat& frame){
    cv::Scalar color( 255, 0, 0 );
    if(centerList.size()>1)
        for(unsigned int i = 1; i<centerList.size() ; i++)
            line(frame, centerList[i].getCenter(), centerList[i-1].getCenter(),color,3);
}
