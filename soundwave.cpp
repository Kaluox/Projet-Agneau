#include "soundwave.h"
#include <QAudioDecoder>


Soundwave::Soundwave(QWidget *parent)
    : QCustomPlot(parent)
    , decoder(new QAudioDecoder(this))
{
    wavePlot = addGraph();
    setMinimumHeight(50);
    connect(decoder, SIGNAL(bufferReady()), this, SLOT(setBuffer()));
    connect(decoder, SIGNAL(finished()), this, SLOT(plot()));
}

void Soundwave::setSource(const QString &fileName) {
    samples.clear();
    decoder->setSourceFilename(fileName);
    decoder->start();
}

Soundwave::~Soundwave() {

}
QAudioDecoder* Soundwave::getDecoder(){
    return decoder;
}
double Soundwave::getPeakValue(const QAudioFormat& format) {
    qreal ret(0);
      if (format.isValid()){
          switch (format.sampleType()) {
              case QAudioFormat::Unknown:
              break;
              case QAudioFormat::Float:
                  if (format.sampleSize() != 32) // other sample formats are not supported
                      ret = 0;
                  else
                      ret = 1.00003;
              break;
              case QAudioFormat::SignedInt:
                  if (format.sampleSize() == 32)
  #ifdef Q_OS_WIN
                      ret = INT_MAX;
  #endif
  #ifdef Q_OS_UNIX
                      ret = SHRT_MAX;
  #endif
                  else if (format.sampleSize() == 16)
                      ret = SHRT_MAX;
                  else if (format.sampleSize() == 8)
                      ret = CHAR_MAX;
                  break;
              case QAudioFormat::UnSignedInt:
                  if (format.sampleSize() == 32)
                      ret = UINT_MAX;
                  else if (format.sampleSize() == 16)
                      ret = USHRT_MAX;
                  else if (format.sampleSize() == 8)
                      ret = UCHAR_MAX;
              break;
          default:
              break;
          }
      }
      return ret;
}


void Soundwave::setBuffer() {
    buffer = decoder->read();
    qreal peak = getPeakValue(buffer.format());
    const qint16 *data = buffer.constData<qint16>();
    int count = buffer.sampleCount() / 2;
    qDebug() << "count : " << count << "samples : " << buffer.sampleCount();
    for (int i=0; i<count; i++){
        double val = data[i]/peak;
        samples.append(val);
        qDebug() << "cheh";
    }
}

void Soundwave::plot() {
    QVector<double> x(samples.size());
    for (int i=0; i<x.size(); i++) {
       x[i] = i;
    }
    wavePlot->addData(x, samples);
    yAxis->setRange(QCPRange(-1, 1));
    xAxis->setRange(QCPRange(0, samples.size()));
    replot();
}
