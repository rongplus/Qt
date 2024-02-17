
#include "heatmapper.h"
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QRadialGradient>
#include <QDebug>
#include "gradientpalette.h"


HeatMapper::HeatMapper(QImage *image, GradientPalette *palette,
					   int radius, int opacity)
	: radius_(radius), opacity_(opacity), max_(1)
{
	Q_ASSERT(image);
	Q_ASSERT(palette);

	palette_ = palette;
	
	mainCanvas_ = image;
	alphaCanvas_ = new QImage(image->size(), QImage::Format_ARGB32);
	Q_ASSERT(alphaCanvas_);
	alphaCanvas_->fill(QColor(0, 0, 0, 0));

	width_ = image->width();
	height_ = image->height();
	data_.resize(width_ * height_);
	data_.fill(0);
	rdata_.resize(width_ * height_);
	rdata_.fill(100);
}

HeatMapper::~HeatMapper()
{}


int HeatMapper::increase(int x, int y, int rad, int delta )
{
	int index = (y - 1) * width_ + (x - 1);
	data_[index] += delta;
	rdata_[index] = rad;
	return data_[index];
}


void HeatMapper::save(const QString &fname)
{
	
}


void HeatMapper::addPoint(int x, int y, int rad)
{
	if (x <= 0 || y <= 0 || x > width_ || y > height_)
		return;

	int count = increase(x, y, 1, rad);

	if (max_ < count) 
	{
		max_ = count;
		redraw();
		return;
	}

	drawAlpha(x, y, count, rad);
}


void HeatMapper::redraw()
{
	QColor color(0, 0, 0, 0);
	
	alphaCanvas_->fill(color);
	mainCanvas_->fill(color);

	int size = data_.size();
	for (int i = 0; i < size; ++i) {
		if (0 == data_[i])
			continue;
		drawAlpha(i % width_ + 1, i / width_ + 1, data_[i], rdata_[i], false);
	}
	colorize();
}


void HeatMapper::setPalette(GradientPalette *palette)
{
	Q_ASSERT(palette);
	
	if (palette)
		palette_ = palette;
}


int HeatMapper::getCount(int x, int y)
{
	if (x < 0 || y < 0)
		return 0;
	return data_[(y - 1) * width_ + (x - 1)];
}


void HeatMapper::drawAlpha(int x, int y, int count, int rad, bool colorize_now)
{
	int alpha = int(qreal(count * 1.0 / max_)*255);
	QRadialGradient gradient(x, y, rad);
	gradient.setColorAt(0, QColor(0, 0, 0, alpha));
	gradient.setColorAt(1, QColor(0, 0, 0, 0));
	
	QPainter painter(alphaCanvas_);
	painter.setPen(Qt::NoPen);
	painter.setBrush(gradient);
	painter.drawEllipse(QPoint(x, y), rad, rad);

	if (colorize_now)
		colorize(x, y, rad);
}


void HeatMapper::colorize()
{
	colorize(0, 0, width_, height_);
}


void HeatMapper::colorize(int x, int y, int rad)
{
	int left = x - rad;
	int top = y - rad;
	int right = x + rad;
	int bottom = y + rad;
	QColor color;

	if (left < 0)
		left = 0;

	if (top < 0)
		top = 0;

	if (right > width_)
		right = width_;

	if (bottom > height_)
		bottom = height_;

	colorize(left, top, right, bottom);
}


void HeatMapper::colorize(int left, int top, int right, int bottom)
{
	int alpha = 0;
	int finalAlpha = 0;
	QColor color;
	for (int i = left; i < right; ++i) {
		for (int j = top; j < bottom; ++j) {
			alpha = qAlpha(alphaCanvas_->pixel(i, j));
			if (!alpha)
				continue;
			finalAlpha = (alpha < opacity_ ? alpha : opacity_);
			color = palette_->getColorAt(alpha);
			mainCanvas_->setPixel(i, j, qRgba(color.red(),
											  color.green(),
											  color.blue(),
											  finalAlpha));
		}
	}
}

void HeatMapper::reset_points()
{
	QColor color(0, 0, 0, 0);

	alphaCanvas_->fill(color);
	mainCanvas_->fill(color);
	data_.fill(0,data_.size());
}



















