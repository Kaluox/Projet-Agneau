#include "trajectory.h"

Trajectory::Trajectory()
{

}

void Trajectory::addPoint(Node& node)
{
	centerList.push_back(node);
}

vector<Node>& Trajectory::getCenterList()
{
	return centerList;
}

void Trajectory::setLastCenter(Node& node)
{
	centerBack = node;
}

Node Trajectory::getLastCenter()
{
	return lastCenter;
}

void Trajectory::drawTrajectoire(cv::Mat& frame)
{
	cv::Scalar color(255, 0, 0);

	if(centerList.size()>1)
	{
		for(unsigned int i = 1; i < centerList.size(); i++)
		{
			line(frame, centerList[i].getCenter(), centerList[i - 1].getCenter(), color, 3);
		}
	}
}
