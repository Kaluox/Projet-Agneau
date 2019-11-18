#include "model.h"

// This file regroups the video and the sound to be used. This is made to match the model part of the MVC

Model::Model()
{
	video = new Video();
	sound = new Sound();
}

Model::~Model()
{
	delete video;
	delete sound;
}

Video* Model::getVideo()
{
	return video;
}
Sound* Model::getSound()
{
	return sound;
}
