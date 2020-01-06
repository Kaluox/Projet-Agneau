#ifndef SOUND_H
#define SOUND_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <QAudioDecoder>
#include <QColor>
#include <QDebug>
#include <QMediaPlayer>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QThread>
#include <QWaitCondition>

#include "../view/qcustomplot.h"

#include "soundwave.h"

class Sound
{
	private:
		QString soundFileClass_0 ;
		QString soundFileClass_1 ;
		QString soundFileClass_2 ;
		QString soundFileClass_3 ;
		QString soundFileClass_4 ;
		QString soundFileToCompare ;

		QString resultSoundFile_0 ;
		QString resultSoundFile_1 ;
		QString resultSoundFile_2 ;
		QString resultSoundFile_3 ;
		QString resultSoundFile_4 ;

		QCPItemLine* item;
		QPen curvePen;

		/** A Qt component which allow decoding the sound file. */
		QAudioDecoder* audioDecoder;

		/** A Qt component which contain the audio samples read from the decoder. */
		QAudioBuffer* buffer;

		/** A list of samples obtained from the buffer. */
		QVector<double> samples;

		/** A Qt component which plays the sound. */
		QMediaPlayer* qMediaPlayer ;

		/** An instance of the Soundwave class. */
		Soundwave* soundwave;

	public:
		/**
		 * The constructor by default of the Sound class.
		 */
		Sound();

		/**
		 * The destructor of the Sound class.
		 */
		~Sound();

		/**
		 * A method which extracts the sound part of the video from a video file.
       * @param fileName The path to the sound file.
		 */
		void extractSound(QString filename);
		
      /**
       * A method which sets the file the media player read.
       * @param fileName The path to the sound file.
       */
      void setMedia(QString filename) ;

      /**
       * A method which starts the sound playback.
       */
		void play();

      /**
       * A method which pauses the sound playback.
       */
		void pause();

      /**
       * A method which stops the sound playback.
       */
		void stop();

      /**
       * A method which change the position on the timeline of the sound recording.
       */
		void setPosition(qint64 position) ;

		void initPlotVectors(unsigned int frameCount, QCustomPlot & qCPsoundSignal, QString filename);
		void initCursor(QCustomPlot & qCPsoundSignal);
		void updateCursor(int value, QCustomPlot & qCPsoundSignal);
		static void audio_encode(const char *filename);
		static qreal getPeakValue(const QAudioFormat &format);
		void setSoundFileClass_0(QString filename) ;
		void setSoundFileClass_1(QString filename) ;
		void setSoundFileClass_2(QString filename) ;
		void setSoundFileClass_3(QString filename) ;
		void setSoundFileClass_4(QString filename) ;
		void setSoundFileToCompare(QString filename) ;

		QString getResultSoundFile_0() ;
		QString getResultSoundFile_1() ;
		QString getResultSoundFile_2() ;
		QString getResultSoundFile_3() ;
		QString getResultSoundFile_4() ;
};

#endif // SOUND_H
