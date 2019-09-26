#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <QWidget>
#include "model.h"
#include "view.h"

#include <QDir>

class Controller : QObject{

    Q_OBJECT

    private:
        QWidget* qWidgetMainWindow ;
        QWidget* qWidgetObjectHistogramWindow ;
        QWidget* qWidgetShiftMaxWindow ;
        QWidget* qWidgetVideoThresholdWindow ;
        QWidget* qWidgetColorSelection ;
        QWidget* qWidgetExtractVideoWindow ;

        Model* model ;
        View* view ;
        QString folderName ;
        QString filename ;
        QString currentPathVideos ;
        QString currentPath ;
        QImage startFrame;
        QImage endFrame;
        int positionSlide;

        bool isMousePressed ;
        bool isStopped = false;
        bool hasSound;
    public:
        Controller(Model* model, View* view);
        ~Controller() ;
    public slots:
        void updateVideo(QImage img, int currentFrame, QString info) ;
        void updateThresholdVideo(QImage img) ;

        void applyColor() ;

        void algorithmSubstractionChecked(bool toogled) ;
        void algorithmColorsChecked(bool toogled) ;

        void displayShiftMaxDialog() ;
        void displayObjectHistogram() ;
        void displayVideoThreshold() ;
        void displayColorSelection() ;
        void displayExtractVideo() ;
        void openVideoFile() ;


        void loadTrajectory() ;
        void saveTrajectory() ;
        void clearList() ;

        void sliderVideoMoved(int position) ;
        void sliderVideoPressed() ;
        void sliderVideoReleased();
        void playStopVideo() ;
        void beginningVideo() ;
        void backward() ;
        void fastBackward() ;
        void endVideo() ;
        void forward() ;
        void fastForward() ;

        void trajectoryChecked(int checked) ;
        void visualizeChecked() ;
        void treatmentChecked() ;

        void mousePressed(int x, int y) ;
        void mouseMoving(int x, int y) ;
        void mouseReleased(int x, int y) ;
        void mouseMovingAndPressed(int x, int y) ;

        void shiftMaxEdited() ;

        void extractVideo() ;
        void setStartVideoExtraction() ;
        void setEndVideoExtraction() ;
        void onSoundXRangeChanged(const QCPRange &range);
        void onSoundYRangeChanged(const QCPRange &range);


};

#endif // CONTROLLER_H
