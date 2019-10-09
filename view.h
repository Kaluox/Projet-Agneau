#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QColorDialog>
#include "mainwindow.h"
#include "shiftmaxwindow.h"
#include "objecthisrogramwindow.h"
#include "thresholdvideowindow.h"
#include "extractvideowindow.h"

/**
 * The class which matches the View part of the MVC pattern.
 */
class View
{
    private:
        /** The main window of the application. */
        MainWindow* mainWindow;

        /** ??? */
        ShiftMaxWindow* shiftMaxWindow ;

        /** A window which display the area of the video which match the color selected using the Qt component. */
        ObjectHistogramWindow* objectHistogramWindow;

        /** ??? */
        ThresholdVideoWindow* thresholdVideoWindow;

        /** A Qt component allowing the selection of a color which will be used by the application to track the movements of the lamb. */
        QColorDialog* qColorDialog;

        /** Export a part of the video as a video itself. */
        ExtractVideoWindow* extractVideoWindow;

        std::vector<QWidget*> views;

    public:
        /**
         * The constructor by default of the View class.
         */
        View();

        /**
         * The destructor of the View class.
         */
        ~View();
        QWidget* getMainWindow();
        QWidget* getShiftMaxWindow();
        QWidget* getObjectHistogramWindow();
        QWidget* getThresholdVideoWindow();
        QWidget* getQColorDialog();
        QWidget* getExtractVideoWindow();
};

#endif // VIEW_H
