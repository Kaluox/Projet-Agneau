#include "extractvideowindow.h"

/**
 * This class is used to create the popup window asking the confirmation on the extraction of the video.
 */

/// Constructor of ExtractVideoWindow
ExtractVideoWindow::ExtractVideoWindow(){
    qLabel = new QLabel ;
    qPushButton = new QPushButton("Confirmer") ;
    qVBoxLayout = new QVBoxLayout ;
    init() ;
}


/// Initalize the window with a push button and a label
void ExtractVideoWindow::init(){
    setModal(true);
    qVBoxLayout->addWidget(qLabel);
    qVBoxLayout->addWidget(qPushButton) ;

    setLayout(qVBoxLayout);
}


///Set the text of the label given a QString
void ExtractVideoWindow::setQLabel(QString q){qLabel->setText(q);}


///Get the instance of the push button
QPushButton* ExtractVideoWindow::getQPushButton(){return qPushButton ;}


///Deletes the label and the push button
ExtractVideoWindow::~ExtractVideoWindow(){
    delete qLabel ;
    delete qPushButton ;
}

