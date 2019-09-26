#ifndef SHIFTMAXWINDOW_H
#define SHIFTMAXWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

class ShiftMaxWindow : public QDialog{
    private:
        QHBoxLayout* qHBoxLayout ;
        QLabel* qLabel ;
        QLineEdit* qLineEdit ;
        QPushButton* qPushButton ;

        void init() ;
    public:
        ShiftMaxWindow();
        QLineEdit* getQLineEdit() ;
        QPushButton* getQPushButton() ;
        ~ShiftMaxWindow() ;
};

#endif // SHIFTMAXWINDOW_H
