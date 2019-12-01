#include "thresholdvideowindow.h"

ThresholdVideoWindow::ThresholdVideoWindow(){
    qLabelThreshold = new QLabel ;
    qVBoxlayout = new QVBoxLayout ;

    init();
}

void ThresholdVideoWindow::init(){

    setFixedSize(600,400);
    qVBoxlayout->addWidget(qLabelThreshold);

    setLayout(qVBoxlayout);
}

void ThresholdVideoWindow::displayImage(QImage img){
    qLabelThreshold->setPixmap(QPixmap::fromImage(img).scaled(qLabelThreshold->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
    qLabelThreshold->adjustSize();
}

ThresholdVideoWindow::~ThresholdVideoWindow(){

    delete qVBoxlayout;
    delete qLabelThreshold;
}

