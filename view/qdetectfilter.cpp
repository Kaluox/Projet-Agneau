#include "qdetectfilter.h"

QDetectFilter::QDetectFilter()
{

}

QVideoFilterRunnable* QDetectFilter::createFilterRunnable()
{
    return new QDetectFilterRunnable(this);
}
