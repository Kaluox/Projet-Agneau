#ifndef OBJECTHISTOGRAMWINDOW_H
#define OBJECTHISTOGRAMWINDOW_H


#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

class ObjectHistogramWindow : public QDialog
{
    private :
        QVBoxLayout* qVBoxLayout;

        QHBoxLayout* qHboxLayoutColorInformation;
        QLabel* qLabelRed;
        QLabel* qLabelRedMin;
        QLabel* qLabelRedMax;

        QLabel* qLabelGreen;
        QLabel* qLabelGreenMin;
        QLabel* qLabelGreenMax;

        QLabel* qLabelBlue;
        QLabel* qLabelBlueMin;
        QLabel* qLabelBlueMax;

        QHBoxLayout* qHboxLayoutHist;
        QLabel* qLabelRedHist;
        QLabel* qLabelGreenHist;
        QLabel* qLabelBlueHist;

        /**
         * A method which draws the histogram.
         */
        void init();

    public:
        /**
         * The constructor by default of the ObjectHistogramWindow class.
         */
        ObjectHistogramWindow();

        /**
         * The destructor of the ObjectHistogramWindow class.
         */
        ~ObjectHistogramWindow();

        /**
         * A method which draws the legend of the histogram.
         */
        void setQLabelHistogram(QImage redHis, QImage greenHist, QImage blueHist, int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax);
};

#endif // OBJECTHISTOGRAMWINDOW_H
