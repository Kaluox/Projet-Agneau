#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "node.h"

/**
 * The class which stores the trajectory of a lamb in a video.
 */
class Trajectory
{
    private:
        /** The trajectory, stored as a list of Nodes objects. */
        std::vector<Nodes> centerList;

        /** The last value of the array 'center'. */
        Nodes centerBack;

    public:
        /**
         * The constructor by default of the Trajectory class.
         */
        Trajectory();

        /**
         * A method which adds a point to the trajectory.
         * @param node The point that is added to the trajectory.
         */
        void addPoint(Nodes& node);

        /**
         * A method which draws the trajectory in an OpenCV matrix.
         * @param frame The matrice which will be drawn on.
         */
        void drawTrajectoire(cv::Mat& frame);

        /**
         * A method which returns the trajectory as a list of Nodes objects.
         * @return The trajectory.
         */
        std::vector<Nodes>& getCenterList();

        /**
         * Set the 'centerBack' attribute to the value passed as an argument.
         * @param node The new value of the 'centerBack' attribute.
         */
        void setLastCenter(Nodes& node);

        /**
         * Get the value of the 'centerBack' attribute.
         * @return The value of the 'centerBack' attribute.
         */
        Nodes getLastCenter();
};

#endif // TRAJECTORY_H
