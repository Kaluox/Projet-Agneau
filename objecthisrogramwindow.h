#ifndef OBJECTHISTOGRAMWINDOW_H
#define OBJECTHISTOGRAMWINDOW_H


#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

class ObjectHistogramWindow : public QDialog{
    private :
        QVBoxLayout* qVBoxLayout ;

        QHBoxLayout* qHboxLayoutColorInformation ;
        QLabel* qLabelRed ;
        QLabel* qLabelRedMin ;
        QLabel* qLabelRedMax ;

        QLabel* qLabelGreen ;
        QLabel* qLabelGreenMin ;
        QLabel* qLabelGreenMax ;

        QLabel* qLabelBlue ;
        QLabel* qLabelBlueMin ;
        QLabel* qLabelBlueMax ;

        QHBoxLayout* qHboxLayoutHist ;
        QLabel* qLabelRedHist ;
        QLabel* qLabelGreenHist ;
        QLabel* qLabelBlueHist ;

        void init() ;
    public:
        ObjectHistogramWindow();
        void setQLabelHistogram(QImage redHis, QImage greenHist, QImage blueHist, int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax) ;
        ~ObjectHistogramWindow() ;
};

#endif // OBJECTHISTOGRAMWINDOW_H
