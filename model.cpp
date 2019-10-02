#include "model.h"

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
