#ifndef NODE_H
#define NODE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDateTime>

/**
 * The class which stores a point of a trajectory.
 */
class Nodes
{
	private:
		/** The coordinates of the point on the video. */
		cv::Point2f center;

		/** The time of the video matching the point. */
		QDateTime timeNow;

		/** The number of the frame. */
		int tmpCurrentFrame;

	public:
		/**
		 * The constructor by default of the Nodes class.
		 */
		Nodes();

		/**
		 * A custom constructor of the Nodes class.
		 * @param center The point on the video.
		 * @param timeNow The time of the video matching the point.
		 * @param tmpCurrentFrame The number of the frame.
		 */
		Nodes(cv::Point2f center, QDateTime timeNow, int tmpCurrentFrame);

		/**
		 * A method which returns a reference to the Point2f OpenCV object storing the coordinates of the points on the video.
		 * @return A reference to the Point2f OpenCV object storing the coordinates of the points on the video.
		 */
		cv::Point2f& getCenter();

		/**
		 * A method which returns a string describing the point.
		 * @return A string describing the point.
		 */
		QString nodeToString();
};

#endif // NODE_H
