#ifndef THRESHOLDVIDEOWINDOW_H
#define THRESHOLDVIDEOWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QColorDialog>
#include <QPushButton>
#include <QSlider>
#include <QString>
class ThresholdVideoWindow : public QDialog{
    private:
        QLabel* qLabelThreshold ;
        QVBoxLayout* qVBoxlayout ;

        void init();

    public:
        ThresholdVideoWindow();
        void displayImage(QImage img) ;

        ~ThresholdVideoWindow() ;
};

#endif // THRESHOLDVIDEOWINDOW_H
