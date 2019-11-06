#ifndef QDETECTFILTER_H
#define QDETECTFILTER_H

#include "qdetectfilterrunnable.h"

class QDetectFilter : public QAbstractVideoFilter
{
	Q_OBJECT

	public:
		/**
		 * The constructor by default of the QDetectFilter class.
		 */
		QDetectFilter();

		/**
		 * A method which create and return an instance of the QVideoFilterRunnable class based on the instance of the QDetectFilter class.
		 */
		QVideoFilterRunnable* createFilterRunnable();
};

#endif // QDETECTFILTER_H
