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
class ThresholdVideoWindow : public QDialog
{
	private:
		QLabel* qLabelThreshold;
		QVBoxLayout* qVBoxlayout;

		/**
		 * A method which initializes the instancied object.
		 */
		void init();

	public:
		/**
		 * The constructor by default of the Model class.
		 */
		ThresholdVideoWindow();
		
		/**
		 * The destructor of the Model class.
		 */
		~ThresholdVideoWindow();

		/**
		 * A method which sets the content of the window to a given image.
		 * @return The image that should be displayed.
		 */
		void displayImage(QImage img);
};

#endif // THRESHOLDVIDEOWINDOW_H
