#ifndef EXTRACTVIDEOWINDOW_H
#define EXTRACTVIDEOWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class ExtractVideoWindow : public QDialog{
    private:

        ///Layout of the window
        QVBoxLayout* qVBoxLayout ;

        ///Label, ie what is written in the window
        QLabel* qLabel ;

        ///Button asking for a confirmation
        QPushButton* qPushButton ;

        /**
         *@brief initializes the window
         */
        void init() ;
    public:

        /**
         * @param q : QString
         * @brief Sets the text on a label shown in the window
         */
        void setQLabel(QString q) ;

        /**
         * @return instance of the push button
         */
        QPushButton* getQPushButton() ;

        /**
         *@brief Default constructor, initializes the window with basic informations
         */
        ExtractVideoWindow();

        /**
         *@brief Destructor, does nothing
         */
        ~ExtractVideoWindow() ;
};

#endif // EXTRACTVIDEOWINDOW_H
