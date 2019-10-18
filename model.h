#ifndef MODEL_H
#define MODEL_H

#include "sound.h"
#include "trajectory.h"
#include "video.h"

/**
 * The class which matches the Model part of the MVC pattern.
 */
class Model
{
	private:
		/** The image part of the video that is processed by the application. */
		Video* video;

		/** The sound part of the video that is processed by the application. */
		Sound* sound;

	public:
		/**
		 * The constructor by default of the Model class.
		 */
		Model();

		/**
		 * The destructor of the Model class.
		 */
		~Model();

		/**
		 * A method which returns the image part of the video that is processed by the application.
		 * @return The image part of the video.
		 */
		Video* getVideo();

		/**
		 * A method which returns the sound part of the video that is processed by the application.
		 * @return The sound part of the video.
		 */
		Sound* getSound();
};

#endif // MODEL_H
