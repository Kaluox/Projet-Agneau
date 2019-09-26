#include "shiftmaxwindow.h"

ShiftMaxWindow::ShiftMaxWindow(){
    qHBoxLayout = new QHBoxLayout ;
    qLabel = new QLabel("Déplacement maximum : ") ;
    qLineEdit = new QLineEdit ;
    qPushButton = new QPushButton("Confirmer") ;

    init() ;
}

void ShiftMaxWindow::init(){
    setModal(true);
    qHBoxLayout->addWidget(qLabel);
    qHBoxLayout->addWidget(qLineEdit);
    qHBoxLayout->addWidget(qPushButton);
    setLayout(qHBoxLayout);
}

QLineEdit* ShiftMaxWindow::getQLineEdit(){return qLineEdit ;}
QPushButton* ShiftMaxWindow::getQPushButton(){return qPushButton ; }

ShiftMaxWindow::~ShiftMaxWindow(){
    delete qPushButton ;
    delete qLineEdit;
    delete qLabel ;
    delete qHBoxLayout ;
}
