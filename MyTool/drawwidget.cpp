#include "drawwidget.h"
#include <QPainter>
#include <QLinearGradient>

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{

}

void DrawWidget::paintEvent(QPaintEvent *e)
{
	QPainter p(this);

	QLinearGradient linearGradient(0,0,rect().width(),rect().height()); //水平渐变
		linearGradient.setColorAt(0.0,QColor(34,120,0));
		linearGradient.setColorAt(0.5,QColor(34,120,0));
		linearGradient.setColorAt(0.6,Qt::transparent);
		linearGradient.setColorAt(1.0,Qt::transparent);
		//p.setBrush(QBrush(Qt::white));
		p.setBrush(QBrush(linearGradient));

		//p.setPen(QPen(QColor(34,120,0), 1));

		p.drawRoundedRect(0,0,rect().width(),rect().height(),10, 10);

		p.setCompositionMode(QPainter::RasterOp_SourceXorDestination  );



		QPixmap pix("/Users/rong/Desktop/Work/13.png");

		p.drawPixmap(rect(),pix,pix.rect());
}
