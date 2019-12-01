#ifndef SOUNDWAVE_H
#define SOUNDWAVE_H

#include <QAudioBuffer>
#include "../view/qcustomplot.h"

class QAudioDecoder;

class Soundwave : public QCustomPlot
{
	Q_OBJECT

	/** Is it necessary to set the scope of the variables to public? */
	public:
		/** A Qt component which allow decoding the sound file. */
		QAudioDecoder* decoder;

		/** A Qt component which contain the audio samples read from the decoder. */
		QAudioBuffer buffer;

		/** A list of samples obtained from the buffer. */
		QVector<double> samples;

		/** A graph created from the samples. */
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
		 * @return A pointer to the QAudioDecoder object.
		 */
		QAudioDecoder* getDecoder();

	public slots:
		/**
		 * A method which set the buffer and the list of samples.
		 */
		void setBuffer();

		/**
		 * A method which create a graph from the list of samples.
		 */
		void plot();
};

#endif // SOUNDWAVE_H
