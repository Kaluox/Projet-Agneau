#include "model.h"

Model::Model(){
    video = new Video() ;
    sound = new Sound() ;

}

Video* Model::getVideo(){return video ;}
Sound* Model::getSound(){return sound ;}

Model::~Model(){
    delete video ;
    delete sound ;
}
