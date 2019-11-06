#include "node.h"

Node::Node()
{
	tmpCurrentFrame = -1;
}

Node::Node(cv::Point2f center, QDateTime timeNow, int tmpCurrentFrame)
{
	this->center = center;
	this->timeNow = timeNow;
	this->tmpCurrentFrame = tmpCurrentFrame;
}

cv::Point2f& Node::getCenter()
{
	return center;
}

QString Node::nodeToString()
{
	char bufferx[50], buffery[50];

	sprintf(bufferx, "%-.2f", center.x);
	sprintf(buffery, "%-.2f", center.y);
	
	return QString("%1;   %2;   %3;   %4").arg(tmpCurrentFrame).arg(bufferx).arg(buffery).arg(timeNow.toString());
}
