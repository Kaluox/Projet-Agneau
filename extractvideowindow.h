#ifndef EXTRACTVIDEOWINDOW_H
#define EXTRACTVIDEOWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class ExtractVideoWindow : public QDialog{
    private:
        QVBoxLayout* qVBoxLayout ;
        QLabel* qLabel ;
        QPushButton* qPushButton ;
        void init() ;
    public:
        void setQLabel(QString q) ;
        QPushButton* getQPushButton() ;
        ExtractVideoWindow();
        ~ExtractVideoWindow() ;
};

#endif // EXTRACTVIDEOWINDOW_H
