#include "objecthistogramwindow.h"

ObjectHistogramWindow::ObjectHistogramWindow(){
    qVBoxLayout = new QVBoxLayout ;
    qHboxLayoutColorInformation = new QHBoxLayout ;
    qLabelRed = new QLabel("R") ;
    qLabelRedMin = new QLabel("N/a");
    qLabelRedMax = new QLabel("N/a") ;

    qLabelGreen = new QLabel("G") ;
    qLabelGreenMin = new QLabel("N/a") ;
    qLabelGreenMax = new QLabel("N/a") ;

    qLabelBlue = new QLabel("B") ;
    qLabelBlueMin = new QLabel("N/a");
    qLabelBlueMax = new QLabel("N/a") ;

    qHboxLayoutHist = new QHBoxLayout ;
    qLabelRedHist = new QLabel ;
    qLabelGreenHist = new QLabel ;
    qLabelBlueHist = new QLabel ;

    init() ;
}

void ObjectHistogramWindow::init(){
    setModal(true);
    setFixedSize(700, 300);

    qLabelRedHist->setStyleSheet("background-color :black;");
    qLabelRedHist->setFixedSize(150,150);
    qHboxLayoutHist->addWidget(qLabelRedHist);

    qLabelGreenHist->setStyleSheet("background-color :black;");
    qLabelGreenHist->setFixedSize(150,150);
    qHboxLayoutHist->addWidget(qLabelGreenHist);

    qLabelBlueHist->setStyleSheet("background-color :black;");
    qLabelBlueHist->setFixedSize(150,150);
    qHboxLayoutHist->addWidget(qLabelBlueHist);

    qVBoxLayout->addLayout(qHboxLayoutHist);

    qHboxLayoutColorInformation->addWidget(qLabelRed);
    qHboxLayoutColorInformation->addWidget(qLabelRedMin);
    qHboxLayoutColorInformation->addWidget(qLabelRedMax);

    qHboxLayoutColorInformation->addWidget(qLabelGreen);
    qHboxLayoutColorInformation->addWidget(qLabelGreenMin);
    qHboxLayoutColorInformation->addWidget(qLabelGreenMax);

    qHboxLayoutColorInformation->addWidget(qLabelBlue);
    qHboxLayoutColorInformation->addWidget(qLabelBlueMin);
    qHboxLayoutColorInformation->addWidget(qLabelBlueMax);

    qVBoxLayout->addLayout(qHboxLayoutColorInformation);

    setLayout(qVBoxLayout);
}

void ObjectHistogramWindow::setQLabelHistogram(QImage redHis, QImage greenHist, QImage blueHist, int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax){
    qLabelRedHist->setPixmap(QPixmap::fromImage(redHis));
    qLabelGreenHist->setPixmap(QPixmap::fromImage(greenHist));
    qLabelBlueHist->setPixmap(QPixmap::fromImage(blueHist));

    qLabelRedMin->setText(QString::number(redMin));
    qLabelRedMax->setText(QString::number(redMax));

    qLabelGreenMin->setText(QString::number(greenMin));
    qLabelGreenMax->setText(QString::number(greenMax));

    qLabelBlueMin->setText(QString::number(blueMin));
    qLabelBlueMax->setText(QString::number(blueMax));

}

ObjectHistogramWindow::~ObjectHistogramWindow(){
    delete qLabelBlueHist ;
    delete qLabelGreenHist ;
    delete qLabelRedHist ;
    delete qHboxLayoutHist ;

    delete qLabelBlueMax ;
    delete qLabelBlueMin ;
    delete qLabelBlue ;

    delete qLabelGreenMax ;
    delete qLabelGreenMin ;
    delete qLabelGreen ;

    delete qLabelRedMax ;
    delete qLabelRedMin ;
    delete qLabelRed ;

    delete qHboxLayoutColorInformation ;
    delete qVBoxLayout ;

}
