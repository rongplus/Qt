
#include "gradientpalette.h"
#include <QImage>
#include <QPainter>
#include <QDebug>

/*
 
 */
GradientPalette::GradientPalette(int width)
	: gradient_(0, 0, width, 1),
	  width_(width)
{
	canvas_ = new QImage(width, 1, QImage::Format_ARGB32);
}

/*
 
 */
GradientPalette::~GradientPalette()
{
	delete canvas_;
	canvas_ = NULL;
}

/*
 
 */
void GradientPalette::setColorAt(qreal index, const QColor &color)
{
	gradient_.setColorAt(index, color);

	QPainter painter(canvas_);
	painter.setBrush(gradient_);
	painter.setPen(Qt::NoPen);
	painter.fillRect(canvas_->rect(), gradient_);
}

/*
 
 */
QColor GradientPalette::getColorAt(qreal index)
{
	index -= 1;
	if (index > width_)
		return Qt::color0;
	return canvas_->pixel(index, 0);
}
