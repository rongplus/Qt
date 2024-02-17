#include "mygraphicsview.h"

#include <QGraphicsSvgItem>

MyGraphicsView::MyGraphicsView( QWidget* p)
	:QGraphicsView (p)
{
	QGraphicsScene* pS = new QGraphicsScene(this);

	setScene(pS);

	QGraphicsSvgItem* svg = new QGraphicsSvgItem( QLatin1String("/Users/rong/Desktop/Work/sample_floor_plan.svg"));

	pS->addItem(svg);

}
