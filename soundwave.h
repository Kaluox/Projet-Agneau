#ifndef SOUNDWAVE_H
#define SOUNDWAVE_H

#include <QAudioBuffer>
#include "qcustomplot.h"

class QAudioDecoder;

class Soundwave : public QCustomPlot
{
	Q_OBJECT

	/** Is it necessary to set the scope of the variables to public? */
	public:
		/** The Qt component which allow decoding the sound file. */
		QAudioDecoder* decoder;

		QAudioBuffer buffer;
		QVector<double> samples;
		QCPGraph* wavePlot;

	public:
		/**
		 * The constructor by default of the Soundwave class.
		 */
		Soundwave(QWidget* parent = Q_NULLPTR);

		/**
		 * The destructor of the Soundwave class.
		 */
		~Soundwave();

		/**
		 * A method which gives the path to the sound file to the QAudioDecoder object.
		 * @param fileName The path to the sound file.
		 */
		void setSource(const QString& fileName);

		double getPeakValue(const QAudioFormat& format);

		/**
		 * A method which returns the QAudioDecoder object.
		 * @return The QAudioDecoder object.
		 */
		QAudioDecoder* getDecoder();

	public slots:
		void setBuffer();
		void plot();
};

#endif // SOUNDWAVE_H
