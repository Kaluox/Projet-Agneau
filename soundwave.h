#ifndef SOUNDWAVE_H
#define SOUNDWAVE_H

#include "qcustomplot.h"
#include <QAudioBuffer>

class QAudioDecoder;

class Soundwave : public QCustomPlot
{
      Q_OBJECT
public:
    Soundwave(QWidget *parent = Q_NULLPTR);
    void setSource(const QString &fileName);
    ~Soundwave();
    double getPeakValue(const QAudioFormat& format);
    QAudioDecoder* getDecoder();

public slots:
    void setBuffer();
    void plot();

public:
    QAudioDecoder *decoder;
    QAudioBuffer buffer;
    QVector<double> samples;
    QCPGraph *wavePlot;
};

#endif // SOUNDWAVE_H
