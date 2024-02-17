//////////////////////////////////////////////////////////////////////////////
// barcode.cpp
// BarCode class draws bar code based on the keys parameters passed in
// to drawBarCode() function
//////////////////////////////////////////////////////////////////////////////
#include "barcode.h"

#define SPACE ' '	// SPACE is added because of Qt4 drawText bug 

#define STUDY_BAR_COUNT   8
#define PLATE_BAR_COUNT   9
#define VISIT_BAR_COUNT   9
#define REGLINE_X	0.5
#define REGLINE_Y	0.5
#define INDENT_X	0.5
#define INDENT_Y	0.125
#define BAR_WIDTH	0.25
#define ON_BAR		0.12
#define OFF_BAR		0.04
#define BAR_HEIGHT	0.25
#define REGLINE_WIDTH	7.5
#define REGLINE_HEIGHT	0.06
#define STUDY_LBL_X	1.0
#define LBL_Y		1.10

/////////////////////////////////////////////////////////////////////////////
// BarCode (Constructor)
/////////////////////////////////////////////////////////////////////////////
BarCode::BarCode()
{
	dpi = 72; // default
}
/////////////////////////////////////////////////////////////////////////////
// setKeys
/////////////////////////////////////////////////////////////////////////////
void BarCode::drawBarCode(QPainter& painter, int study, int plate, int visit, 
		QString slabel, QString plabel, QString vlabel)
{
	QString str;
	painter.setPen(Qt::black);
	//font.setStyleStrategy(QFont::NoAntialias);
	//painter.setFont(font);
	// draw right corner markers 
	// draw left corner markers and top barcode line based on 
	// existance of study, plate and visit in order
	painter.fillRect(QRectF(REGLINE_X*dpi, 
			REGLINE_Y*dpi, 
			REGLINE_WIDTH*dpi, 
			REGLINE_HEIGHT*dpi),Qt::black);
	// Draw barcode and label for study
	if (study >= 0)
	{
		if (!slabel.isEmpty())
		{
			painter.drawText(QPointF(STUDY_LBL_X*dpi,
					LBL_Y*dpi),
					slabel+SPACE);
		}
		makebars(&painter, 
			STUDY_LBL_X,
			study,
			STUDY_BAR_COUNT);
	}
	// Draw barcode and label for plate 
	qreal plate_x = qreal(STUDY_LBL_X) + qreal(BAR_WIDTH*STUDY_BAR_COUNT);
	if (plate >= 0)
	{
		if (!plabel.isEmpty())
		{
			painter.drawText(QPointF(plate_x*dpi,
					LBL_Y*dpi),
					plabel+SPACE);
		}
		makebars(&painter, 
			plate_x,
			plate,
			PLATE_BAR_COUNT);

	}
	// Draw barcode and label for visit
	qreal visit_x = plate_x + (BAR_WIDTH*PLATE_BAR_COUNT);
	if (visit >=0)
	{
		if (!vlabel.isEmpty())
		{
			painter.drawText(QPointF(visit_x*dpi,
					LBL_Y*dpi),
					vlabel+SPACE);
		}
		makebars(&painter, 
			visit_x,
			visit,
			VISIT_BAR_COUNT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// makebars
/////////////////////////////////////////////////////////////////////////////
void BarCode::makebars(QPainter* p, qreal pos, int val, int size)
{
    qreal pixpos = pos + (qreal(size-1) * qreal(BAR_WIDTH));
    qreal bars_y = REGLINE_Y+INDENT_Y;
    while (size--)
    {
        if (val & 0x01)
        {
            /* bit is 1 so do thick bar */
                p->fillRect(QRectF(pixpos*dpi, bars_y*dpi, 
			ON_BAR*dpi, BAR_HEIGHT*dpi),Qt::black);
        }
        else
        {
            /* bit is zero so do thin bar */
                p->fillRect(QRectF(pixpos*dpi, bars_y*dpi, OFF_BAR*dpi,
                        BAR_HEIGHT*dpi),Qt::black);
        }
        val = val >> 1;
        pixpos = pixpos - BAR_WIDTH;
    }
}
/////////////////////////////////////////////////////////////////////////////
// draw Page Title 
/////////////////////////////////////////////////////////////////////////////
void BarCode::drawPageTitle(QPainter& painter, QString title)
{
	QFont orig_font = painter.font();
	QFont font = painter.font();
	font.setPointSize(14);
	painter.setPen(Qt::black);
	painter.setFont(font);
	painter.drawText(QRectF(REGLINE_X*dpi,
			(REGLINE_Y-BAR_HEIGHT-0.02)*dpi,
			REGLINE_WIDTH*dpi,
			(BAR_HEIGHT)*dpi),
			Qt::AlignHCenter|Qt::AlignBottom,
			title);
	painter.setFont(orig_font);
}
/////////////////////////////////////////////////////////////////////////////
// draw Page Title 
/////////////////////////////////////////////////////////////////////////////
void BarCode::drawLeftHeader(QPainter& painter, QString header)
{
	QFont orig_font = painter.font();
	QFont font = painter.font();
	font.setPointSize(10);
	painter.setPen(Qt::black);
	painter.setFont(font);
	painter.drawText(QRectF(REGLINE_X*dpi,
			(REGLINE_Y-BAR_HEIGHT-0.02)*dpi,
			(REGLINE_X+REGLINE_WIDTH)*dpi,
			(BAR_HEIGHT)*dpi),
			Qt::AlignLeft|Qt::AlignBottom,
			header);
	painter.setFont(orig_font);
}
