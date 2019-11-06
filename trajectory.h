#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "node.h"

/**
 * The class which stores the trajectory of a lamb in a video.
 */
class Trajectory
{
    private:
        /** The trajectory, stored as a list of Node objects. */
        std::vector<Node> centerList;

        /** The last value of the array 'center'. */
        Node lastCenter;

    public:
        /**
         * The constructor by default of the Trajectory class.
         */
        Trajectory();

        /**
         * A method which adds a point to the trajectory.
         * @param node The point that is added to the trajectory.
         */
        void addPoint(Node& node);

        /**
         * A method which draws the trajectory in an OpenCV matrix.
         * @param frame The matrice which will be drawn on.
         */
        void drawTrajectoire(cv::Mat& frame);

        /**
         * A method which returns the trajectory as a list of Node objects.
         * @return The trajectory.
         */
        std::vector<Node>& getCenterList();

        /**
         * Set the 'centerBack' attribute to the value passed as an argument.
         * @param node The new value of the 'centerBack' attribute.
         */
        void setLastCenter(Node& node);

        /**
         * Get the value of the 'centerBack' attribute.
         * @return The value of the 'centerBack' attribute.
         */
        Node getLastCenter();
};

#endif // TRAJECTORY_H
