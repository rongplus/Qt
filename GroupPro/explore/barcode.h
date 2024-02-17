//////////////////////////////////////////////////////////////////////////////
// barcode.h
// This class is used by DFbarcodetool and DFqcps
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BARCODE_H
#define BARCODE_H

#include <QPainter>
#include <QString>

class BarCode 
{
public:
	BarCode();
	void setDPI(int val) { dpi = val; }
	void drawBarCode(QPainter& p, int study, int plate=501, int visit=1,
		QString slabel="DataFAX", QString plabel="Plate", QString vlabel="Page");
	void drawPageTitle(QPainter& p, QString title);
	void drawLeftHeader(QPainter& painter, QString header);
private:
	void makebars(QPainter* p, qreal pos, int val, int size);
	int dpi;
};

#endif // BARCODE_H 


