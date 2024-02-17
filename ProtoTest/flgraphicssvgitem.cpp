#include "flgraphicssvgitem.h"
#include <QPainter>


extern const int ZC_ITEM_WIDTH;
extern const int ZC_ITEM_HEIGHT;

FLGraphicsSvgItem::FLGraphicsSvgItem(QGraphicsItem *parent)
	: QGraphicsSvgItem(parent)
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

FLGraphicsSvgItem::FLGraphicsSvgItem(const QString & fileName, QGraphicsItem * parent /*= 0*/)
	:QGraphicsSvgItem(fileName,parent)
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

FLGraphicsSvgItem::~FLGraphicsSvgItem()
{

}

void FLGraphicsSvgItem::setFloor(QPointer<FLFloor> pFloor)
{
	m_pFloor = pFloor;
}

QPointer<FLFloor> FLGraphicsSvgItem::Floor()
{
	return m_pFloor;
}

void FLGraphicsSvgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget/*=0*/)
{
	painter->setOpacity(0.3);
	QGraphicsSvgItem::paint(painter,option,widget);
	return;
	QRectF rt = boundingRect();
	//rt.setY(rt.y()-40);
	painter->save();
	QFont serifFont("Times", 25, QFont::Bold);
	

	painter->setFont(serifFont);
	//painter->drawText(rt,Qt::AlignHCenter ,m_pFloor->getProperty("Name").value.toString());
	 rt = boundingRect();
// 	if(isActive())
// 		painter->drawRect(rt);
	int nLen = 10;
	int nMin = qMin(rt.width(),rt.height());
	if(nMin < nLen*2)
		nLen = nMin/2;

	//DrawCornor();
	if(nLen<4)
		return;
	QPen pen;
	pen.setWidth(3);
	painter->setPen(pen);
	QPointF p1, p2;
	//left-top	
	p1 = p2 = rt.topLeft();	
	p2.setX(p2.x() + nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(2,2);
	p2 += QPoint(0,2);
	painter->drawLine(p1,p2);

	p1 = p2 = rt.topLeft();	
	p2.setY(p2.y()+ nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(2,2);
	p2 += QPoint(2,0);
	painter->drawLine(p1,p2);

	//top-right
	
	p1 = p2 = rt.topRight();	
	p2.setX(p2.x() - nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(-2,2);
	p2 += QPoint(0,2);
	painter->drawLine(p1,p2);

	p1 = p2 = rt.topRight();	
	p2.setY(p2.y()+ nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(-2,2);
	p2 += QPoint(-2,0);
	painter->drawLine(p1,p2);
	//bottom-left
	p1 = p2 = rt.bottomLeft();	
	p2.setX(p2.x() + nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(+2,-2);
	p2 += QPoint(0,-2);
	painter->drawLine(p1,p2);

	p1 = p2 = rt.bottomLeft();	
	p2.setY(p2.y() - nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(2,-2);
	p2 += QPoint(2,0);
	painter->drawLine(p1,p2);
	//bottom-right
	p1 = p2 = rt.bottomRight();	
	p2.setX(p2.x() - nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(-2,-2);
	p2 += QPoint(0,-2);
	painter->drawLine(p1,p2);

	p1 = p2 = rt.bottomRight();	
	p2.setY(p2.y() - nLen);
	painter->drawLine(p1,p2);
	p1 += QPoint(-2,-2);
	p2 += QPoint(-2,0);
	painter->drawLine(p1,p2);

	int num_zc = m_pFloor->countZC();
	auto plotting_scale = m_pFloor->getProperty("Plotting_Scale").value.toString();
	auto scale_list = plotting_scale.split(":");
	float v1 = scale_list[0].toFloat();
	float v2 = scale_list[1].toFloat();
	float scale = v1/v2;
	for(int n=0; n< num_zc; n++)
	{
		auto zc = m_pFloor->getZC(n);
		
		QPointF pt = zc->getProperty("Position").value.toPointF();
		pt += QPointF(24,24);
		int range = zc->getProperty("Range").value.toInt() * scale;
		painter->drawEllipse(pt,range,range);
	}
	painter->restore();
}
