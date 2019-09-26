#ifndef MODEL_H
#define MODEL_H
#include "video.h"
#include "sound.h"
#include "trajectory.h"

class Model{
    private:
        Video* video ;
        Sound* sound ;
    public:
        Model() ;
        Video* getVideo() ;
        Sound* getSound() ;
        ~Model() ;
};

#endif // MODEL_H
