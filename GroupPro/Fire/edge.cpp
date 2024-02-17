/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "edge.h"
#include "fldeviceitem.h"

#include <math.h>

#include <QPainter>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

//! [0]
Edge::Edge(FLDeviceItem *sourceNode, FLDeviceItem *destNode)
    : arrowSize(6)
{
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
	set_number("1", "1");
	setZValue(sourceNode->zValue() - 20);
}
//! [0]

//! [1]
FLDeviceItem *Edge::sourceNode() const
{
    return source;
}

FLDeviceItem *Edge::destNode() const
{
    return dest;
}
void Edge::set_number(QString src, QString dest)
{
	_src = src;
	_dest = dest;
}
//! [1]

//! [2]
void Edge::adjust()
{
    if (!source || !dest)
        return;
	int x1 = source->boundingRect().width() / 2;
	int y1 = source->boundingRect().height() / 2;
	int x2 = dest->boundingRect().width() / 2;
	int y2 = dest->boundingRect().height() / 2;

    QLineF line(mapFromItem(source, x1, y1), mapFromItem(dest, x2, y2));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}
//! [2]

//! [3]
QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [3]

//! [4]
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
//! [4]

//! [5]
    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  //  painter->drawLine(line);
//! [5]

//! [6]
    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;
	float sc = 1.5;

   
	int len = 24;
	QPointF s1 = sourcePoint + QPointF(sin(angle + Pi / 3) * len,	cos(angle + Pi / 3) * len);
	QPointF d1 = destPoint + QPointF(sin(angle - Pi / 3) * len,	cos(angle - Pi / 3) * len);

	QLineF line2(s1, d1);
	painter->drawLine(line2);


	QPointF sourceArrowP1 = s1 + QPointF(sin(angle + Pi / 3) * arrowSize*sc,
		cos(angle + Pi / 3) * arrowSize*sc);
	QPointF sourceArrowP2 = s1 + QPointF(sin(angle + Pi - Pi / 3) * arrowSize*sc,
		cos(angle + Pi - Pi / 3) * arrowSize*sc);
	QPointF destArrowP1 = d1 + QPointF(sin(angle - Pi / 3) * arrowSize * sc,
		cos(angle - Pi / 3) * arrowSize * sc);
	QPointF destArrowP2 = d1 + QPointF(sin(angle - Pi + Pi / 3) * arrowSize * sc,
		cos(angle - Pi + Pi / 3) * arrowSize * sc);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << s1 << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << d1 << destArrowP1 << destArrowP2);

	painter->save();
	int offset = (sourceArrowP1.x() - destArrowP2.x() < 0) ? 1 : -1;
	//painter->rotate(angle);
	painter->drawText(sourceArrowP2+ offset* QPointF(15,15), _src);
	painter->drawText(destArrowP2 - offset* QPointF(15, 15), _dest);
	painter->restore();
}
//! [6]
