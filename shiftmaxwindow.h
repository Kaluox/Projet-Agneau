#ifndef SHIFTMAXWINDOW_H
#define SHIFTMAXWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

class ShiftMaxWindow : public QDialog
{
	private:
		QHBoxLayout* qHBoxLayout ;
		QLabel* qLabel ;
		QLineEdit* qLineEdit ;
		QPushButton* qPushButton ;

		/**
		* A method which draws the window.
		*/
		void init();
	public:
		/**
		* The constructor by default of the ShiftMaxWindow class.
		*/
		ShiftMaxWindow();

		/**
		* The destructor of the ShiftMaxWindow class.
		*/
		~ShiftMaxWindow();

		QLineEdit* getQLineEdit() ;
		QPushButton* getQPushButton() ;
};

#endif // SHIFTMAXWINDOW_H
