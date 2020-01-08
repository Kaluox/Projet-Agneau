#ifndef OBJECTINFORMATION_H
#define OBJECTINFORMATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "../../../../../OpenCV/sources/modules/core/include/opencv2/core/types.hpp"

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

        /**
         * A method which returns the raduis of the circle containing the object followed
         * @return Float Radius
         */
		float getRadius();

		/**
		 * Setter of Contours
		 */
		void setContours(std::vector<std::vector<cv::Point>>);

		/**
		 * Setter of the value of the center
		 */
		void setCenter(cv::Point2f);

		/**
		 * Setter of the value of the radius
		 */
		void setRadius(float);
};

#endif // OBJECTINFORMATION_H
