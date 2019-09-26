#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QSlider>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QStatusBar>
#include <QCheckBox>
#include <QMouseEvent>
#include "QCustomPlot/qcustomplot.h"
#include "soundwave.h"
class QLabelVideoDisplay : public QLabel{
    Q_OBJECT
    public:
        QLabelVideoDisplay(){
            setMouseTracking(true);
            setStyleSheet("background-color :black;");
            setMinimumSize(400,245);
            setMaximumSize(800,490);
            setEnabled(false);
            setMouseTracking(true);
        }
    signals:
        void mousePressed(int x, int y) ;
        void mouseMoved(int x, int y) ;
        void mouseReleased(int x, int y) ;
        void mousePressedAndMoving(int x, int y) ;

    protected:
        void mouseMoveEvent(QMouseEvent *evt){
            int x = evt->x() ;
            int y = evt->y() ;
            setCursor(Qt::CrossCursor);
            if(evt->buttons() & Qt::LeftButton)
                emit mousePressedAndMoving(x,y);
            else
                emit mouseMoved(x,y);
        }
        void mousePressEvent(QMouseEvent *evt){emit mousePressed(evt->x(), evt->y());}
        void mouseReleaseEvent(QMouseEvent *evt){emit mouseReleased(evt->x(), evt->y());}
};

class MainWindow : public QMainWindow{
    Q_OBJECT

    private:
        // USER INTERFACE
        QWidget* qWidget ;

        QMenu* qMenuFile ;

        QMenu* qMenuAlgorithme ;
        QMenu* qMenuAlgorithmeCouleur;


        QMenu* qMenuSetting ;
        QMenu* qMenuInformation ;
        QMenu* qMenueAlgorithms ;
        QMenu* qMenueTools ;

        QAction* qActionOpenVideo;
        QAction* qActionQuit ;
        QAction* qOnpenCouleur;
        QAction* qActionOpenColorSelections ;

        QAction* qActionShiftMax ;
        QAction* qActionObjectHistogram ;
        QAction* qActionAlgorithmSubstraction ;
        QAction* qActionAlgorithmColors ;
        QAction* qActionOpenThresholdVideo ;
        QAction* qActionOpenColorSelection ;
        QAction* qActionExtractVideo ;


        QLabelVideoDisplay* qLabelVideoDisplay ;

        QPushButton* qPushButtonPlayPause ;
        QPushButton* qPushButtonFastForward ;
        QPushButton* qPushButtonFastBackward ;
        QPushButton* qPushButtonForward ;
        QPushButton* qPushButtonBackward ;
        QPushButton* qPushButtonEnd ;
        QPushButton* qPushButtonStart ;
        QPushButton* qPushButtonStartVideoToExtract ;
        QPushButton* qPushButtonEndVideoToExtract ;
        QLabel* qLabelStartVideoToExtract ;
        QLabel* qLabelEndVideoToExtract ;

        QPushButton* qPushButtonSaveCoordinates ;
        QPushButton* qPushButtonLoadCoordinates ;
        QPushButton* qPushButtonClearList ;

        QSlider* qSlider ;
        QLabel* qLabelTimestampCurrent ;
        QLabel* qLabelTimestampFinish ;

        // sound signal plot
        QCustomPlot* qCPsoundSignal;
        QHBoxLayout* qHBoxLayoutSound;
        Soundwave* soundwave;
        QVBoxLayout* qVBoxLayout ;
        QVBoxLayout* qVBoxLayoutCoordinates ;

        QHBoxLayout* qHBoxLayoutGroupBox ;
        QHBoxLayout* qHBoxLayoutVideo ;
        QHBoxLayout* qHBoxLayoutVideoControl ;
        QHBoxLayout* qHboxLayoutLoadSaveButton ;
        QHBoxLayout* qHBoxLayoutSlider ;
        QHBoxLayout* qHBoxLayoutExtraction ;
        QHBoxLayout* qHBoxLayoutTrajectory ;

        QLabel* qLabelTrajectory ;
        QCheckBox* qCheckBox ;

        QListWidget* qListWidgetCoordinates ;
        QGroupBox* qGroupBox ;
        QRadioButton* qRadioButtonVisualize ;
        QRadioButton* qRadioButtonTreatment ;

        QStatusBar* qStatusBar ;

        void init() ;

    public:
        MainWindow(QWidget *parent = 0) ;
        QAction* getQActionShiftMax() ;
        QAction* getQActionObjectHistogram() ;
        QAction* getQActionOpenVideo() ;
        QAction* getQActionQuit() ;
        QAction* getQActionAlgorithmSubstraction() ;
        QAction* getQActionAlgorithmColors() ;
        QAction* getQActionOpenThresholdVideo() ;
        QAction* getQActionOpenColorSelection() ;
        QAction* getQActionExtractVideo() ;

        QAction* getQActionOpenColorSelections() ;

        QPushButton* getQPushButtonPlayPause() ;
        QPushButton* getQPushButtonFastForward() ;
        QPushButton* getQPushButtonFastBackward() ;
        QPushButton* getQPushButtonForward() ;
        QPushButton* getQPushButtonBackward() ;
        QPushButton* getQPushButtonEnd() ;
        QPushButton* getQPushButtonStart() ;
        QPushButton* getQPushButtonSaveCoordinate() ;
        QPushButton* getQPushButtonLoadCoordinate() ;
        QPushButton* getQPushButtonClear() ;
        QPushButton* getQPushButtonStartVideoToExtract() ;
        QPushButton* getQPushButtonEndVideoToExtract() ;


        QCheckBox* getQCheckBox() ;
        QGroupBox* getQGroupBox() ;
        QRadioButton* getQRadioButtonVisualize() ;
        QRadioButton* getQRadioButtonTreatment() ;

        QLabelVideoDisplay *getQLabelVideoDisplay() ;
        QSlider* getQSlider() ;
        QListWidget* getQListWidget() ;
        QLabel* getQLabelStartVideoToExtract() ;
        QLabel* getQLabelEndVideoToExtract() ;

        QCustomPlot* getQCPsoundSignal();
        void enableVideoButtons(bool b);
        void setPlayPauseButton(QString qs) ;
        void enableControlButtons(bool b) ;
        void updateSatusBar(QString msg) ;
        void displayImageQLabelVideo(QImage img) ;
        void setQLabelCurrentTime(QString q) ;
        void setQLabelFinishTime(QString q) ;
        void setQLabelStartVideoToExtract(QString q) ;
        void setQLabelEndVideoToExtract(QString q) ;

        ~MainWindow() ;

};


#endif // MAINWINDOW_H
