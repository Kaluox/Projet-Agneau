#ifndef QDETECTFILTER_H
#define QDETECTFILTER_H
#include "qdetectfilterrunnable.h"

class QDetectFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    QDetectFilter();
    QVideoFilterRunnable* createFilterRunnable();
};

#endif // QDETECTFILTER_H
