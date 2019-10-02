#include "node.h"

Nodes::Nodes()
{
	tmpCurrentFrame = -1;
}

Nodes::Nodes(cv::Point2f center, QDateTime timeNow, int tmpCurrentFrame)
{
	this->center = center;
	this->timeNow = timeNow;
	this->tmpCurrentFrame = tmpCurrentFrame;
}

cv::Point2f& Nodes::getCenter()
{
	return center;
}

QString Nodes::nodeToString()
{
	char bufferx[50], buffery[50];

	sprintf(bufferx, "%-.2f", center.x);
	sprintf(buffery, "%-.2f", center.y);
	
	return QString("%1;   %2;   %3;   %4").arg(tmpCurrentFrame).arg(bufferx).arg(buffery).arg(timeNow.toString());
}
