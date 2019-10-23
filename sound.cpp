#include "sound.h"

#include <iostream>
#include <sstream>

#include <QAudioDecoder>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QObject>
#include <QPainter>
#include <QProcess>
#include <QTextCodec>
#include <QTime>

#include "cascadestylesheetloader.h"
#include <gestionvideo.h>
#include <global.h>
#include <ml/Dtw.h>
#include <model.h>
#include <soundwave.h>
#include <source/FramesCollection.h>
#include <source/generator/SineGenerator.h>
#include <source/WaveFile.h>
#include "source/WaveFileHandler.h"
#include <tools/TextPlot.h>
#include <transform/Mfcc.h>

Sound::Sound()
{
		//soundwave = new Soundwave();
		qMediaPlayer = new QMediaPlayer();
		audioDecoder = new QAudioDecoder(qMediaPlayer);
		buffer       = new QAudioBuffer();

		CascadeStyleSheetLoader& loader = CascadeStyleSheetLoader::getInstance();
		
		curvePen.setColor(QColor(loader.getProperty("#accentColor")));
}

qreal Sound::getPeakValue(const QAudioFormat& format)
{
	qreal ret = 0;

	if (format.isValid())
	{
		switch (format.sampleType())
		{
			case QAudioFormat::Unknown:
				break;

			case QAudioFormat::Float:
				if (format.sampleSize() != 32)
				{
					ret = 0;
				}
				else
				{
					ret = 1.00003;
				}
				break;

			case QAudioFormat::SignedInt:
				if (format.sampleSize() == 32)
				{
					ret = INT_MAX;
				}
				else if (format.sampleSize() == 16)
				{
					ret = SHRT_MAX;
				}
				else if (format.sampleSize() == 8)
				{
					ret = CHAR_MAX;
				}
				break;

			case QAudioFormat::UnSignedInt:
				if (format.sampleSize() == 32)
				{
					ret = UINT_MAX;
				}
				else if (format.sampleSize() == 16)
				{
					ret = USHRT_MAX;
				}
				else if (format.sampleSize() == 8)
				{
					ret = UCHAR_MAX;
				}
				break;

			default:
				break;
		}
	}

	return ret;
}

void Sound::extractSound(QString filename)
{
		std::string file      = filename.toStdString() + ".mp4";
		std::string fileAudio = filename.toStdString() + ".wav";

		GestionVideo gestionExtractAudio(file);

		gestionExtractAudio.extractAudio(fileAudio);
}

void Sound::initPlotVectors(unsigned int frameCount, QCustomPlot & qCPsoundSignal, QString filename)
{
		unsigned int width = frameCount;
		//int height = 120;
		QVector<double> x, y;
		QString fileIn = filename + ".wav";
		Aquila::WaveFile wav(fileIn.toStdString());
		//int max = 0;
		unsigned int total = wav.getSamplesCount();
		qDebug() << "lenght : " <<qMediaPlayer->duration();
		qDebug() << "samples : " << wav.getSamplesCount();
		//qDebug() << "bytespersec : " << wav.getBytesPerSec();
		//qDebug() << "channels : " << wav.getChannelsNum();
		//qDebug() << "size : " << wav.getWaveSize();
		//qDebug() << "frequence : " << wav.getSampleFrequency();
		//qDebug() << wav.getSampleFrequency()*28;
		//int maxY = 0;
		qDebug() << fileIn;
		audioDecoder->setSourceFilename("video.wav");
		qDebug() << audioDecoder->sourceFilename();
		//buffer = audioDecoder->read();
		//qreal peak = getPeakValue(buffer.format());
		//const qint16 *data = buffer.constData<qint16>();
		//int count = buffer.sampleCount() / 2;
		//for (int i = 0; i < count; i += 1200){ // I want 40 samples per second currently assuming 48kHz
		//	 double val = data[i]/peak;
		//	 samples.append(val * 300); // *300 for scaling
		//}
		//qDebug() << samples.size();
		const double sampleCount = double(total);

		for (unsigned int i = 0; i < total; i++)
		{
				if (i%10 != 0) continue;
				x.push_back(double(i * width) / sampleCount);
				y.push_back(double(wav.sample(i)) / double(300));
		}
		//qDebug() << "samples : " << soundwave->samples.size();
		QCPCurve* soundCurve = new QCPCurve(qCPsoundSignal.xAxis, qCPsoundSignal.yAxis);
		soundCurve->setData(x,y);
		qCPsoundSignal.setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
		qCPsoundSignal.rescaleAxes();
		qCPsoundSignal.plottable()->setPen(curvePen);
		//qCPsoundSignal.setVisible(true);
		//qDebug() << qCPsoundSignal.itemCount();
		qCPsoundSignal.replot();
		//cursor layer
		qCPsoundSignal.addLayer("cursorLayer", 0, QCustomPlot::limAbove);
		QCPLayer* cursorLayer = qCPsoundSignal.layer("cursorLayer");
		cursorLayer = qCPsoundSignal.layer("cursorLayer");
		cursorLayer->setMode(QCPLayer::lmBuffered);
}


void Sound::initCursor(QCustomPlot & qCPsoundSignal){
		this->item = new QCPItemLine(&qCPsoundSignal);
		item->setPen(QPen(Qt::red));
		item->start->setCoords(1,-120);
		item->end->setCoords(1,120);
		item->setLayer("cursorLayer"); // on place le curseur dans une autre couche
		qCPsoundSignal.replot();
}

void Sound::updateCursor(int value, QCustomPlot & qCPsoundSignal){
		this->item->start->setCoords(value, -120);
		this->item->end->setCoords(value, 120);
		qCPsoundSignal.layer("cursorLayer")->replot();
}

void Sound::setMedia(QString filename)
{
		qMediaPlayer->setMedia(QUrl::fromLocalFile(filename));
		qMediaPlayer->setVolume(100);
}

void Sound::play()
{
	qMediaPlayer->play();
}

void Sound::pause()
{
	qMediaPlayer->pause();
}

void Sound::stop()
{
	qMediaPlayer->stop();
}

void Sound::setPosition(qint64 position)
{
	qMediaPlayer->setPosition(position);
}

void Sound::setSoundFileClass_0(QString filename){soundFileClass_0 = filename ;}
void Sound::setSoundFileClass_1(QString filename){soundFileClass_1 = filename ;}
void Sound::setSoundFileClass_2(QString filename){soundFileClass_2 = filename ;}
void Sound::setSoundFileClass_3(QString filename){soundFileClass_3 = filename ;}
void Sound::setSoundFileClass_4(QString filename){soundFileClass_4 = filename ;}
void Sound::setSoundFileToCompare(QString filename){soundFileToCompare = filename ;}

QString	Sound::getResultSoundFile_0(){return resultSoundFile_0 ;}
QString	Sound::getResultSoundFile_1(){return resultSoundFile_1 ;}
QString	Sound::getResultSoundFile_2(){return resultSoundFile_2 ;}
QString	Sound::getResultSoundFile_3(){return resultSoundFile_3 ;}
QString	Sound::getResultSoundFile_4(){return resultSoundFile_4 ;}

Sound::~Sound(){ delete qMediaPlayer ; }
