#ifndef OBJECTINFORMATION_H
#define OBJECTINFORMATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ObjectInformation
{
	private:
		std::vector<std::vector<cv::Point>> contours;
		cv::Point2f center;
		float radius;

	public:
		/**
		 * The constructor by default of the ObjectInformation class.
		 */
		ObjectInformation();

		/**
		 * A method which returns the list of lists of OpenCV Point objects.
		 * @return The list of lists of OpenCV Point objects.
		 */
		std::vector<std::vector<cv::Point>> getContours();

		/**
		 * A method which returns the OpenCV Point that corresponds to the center.
		 * @return The OpenCV Point that corresponds to the center.
		 */
		cv::Point2f getCenter();
		float getRadius();
		void setContours(vector<vector<cv::Point>>);
		void setCenter(cv::Point2f);
		void setRadius(float);
};

#endif // OBJECTINFORMATION_H
