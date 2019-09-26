#include "extractvideowindow.h"

ExtractVideoWindow::ExtractVideoWindow(){
    qLabel = new QLabel ;
    qPushButton = new QPushButton("Confirmer") ;
    qVBoxLayout = new QVBoxLayout ;
    init() ;
}

void ExtractVideoWindow::init(){
    setModal(true);
    qVBoxLayout->addWidget(qLabel);
    qVBoxLayout->addWidget(qPushButton) ;

    setLayout(qVBoxLayout);
}

void ExtractVideoWindow::setQLabel(QString q){qLabel->setText(q);}
QPushButton* ExtractVideoWindow::getQPushButton(){return qPushButton ;}

ExtractVideoWindow::~ExtractVideoWindow(){
    delete qLabel ;
    delete qPushButton ;
}

