#include "bargraph.h"

#include <QPainter>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QRect>
#include <QTextItem>


void BarGraph::set_min_value(int value)
{
	min_val = value;
}

void BarGraph::set_zb(bool z)
{
	zb = z;
}



void BarGraph::set_max_value(int value)
{
	max_val = value;
}

void BarGraph::setShow_tooltip(bool value)
{
    show_tooltip = value;
    if(!value)
        setToolTip("");
    setMouseTracking(value);
}

bool BarGraph::getShow_label() const
{
    return show_label;
}

void BarGraph::setShow_label(bool value)
{
    show_label = value;
}


QBrush BarGraph::getBrush() const
{
    return brush;
}

void BarGraph::setBrush(const QBrush &value)
{
    brush = value;
}

bool BarGraph::getCut_min_value() const
{
    return cut_min_value;
}

void BarGraph::setCut_min_value(bool value)
{
    cut_min_value = value;
}

int BarGraph::getMargin() const
{
    return margin;
}

void BarGraph::setMargin(int value)
{
    margin = value;
}

QString BarGraph::getYlabel() const
{
    return ylabel;
}

void BarGraph::setYlabel(const QString &value)
{
    ylabel = value;
}

BarGraph::BarGraph(QWidget *parent) :
    QWidget(parent)
{
	show_tooltip=false;
	show_label=true;
	cut_min_value=false;
	margin = 10;
	max_val = -45;
	min_val = -105;

	QPalette Pal(palette());
	
	Pal.setColor(QPalette::Background, QColor(0x5d, 0x5b, 0x59) );
	setAutoFillBackground(true);
	setPalette(Pal);

	cols.resize(50);

	for(int i=0; i<50;i++)
		cols[i] = QColor(qrand() % 255, qrand() % 255, qrand() % 255, 0x50);

	brush = QColor(0, 0xff, 0, 0x50);
	zb = false;

	old_val_index = 0;
}

void BarGraph::setData(std::vector<int> channel, std::vector<int> val,std::vector<QString> labels, bool z)
{
	channels =std::move(channel);
    values =std::move(val);
    label = std::move(labels);
	zb = z;

	if (zb) {

		


		if (min_values.size() == 0) {
			min_values = values;
			max_values = values;
			avg_values = values;
		}
		else {
			std::transform(min_values.begin(), min_values.end(), values.begin(), min_values.begin(), [](int a, int b) { return a < b ? a : b; });
			std::transform(max_values.begin(), max_values.end(), values.begin(), max_values.begin(), [](int a, int b) { return a > b ? a : b; });

		}

		if (old_val_index < 10) {
			old_values[old_val_index] = values;
			old_val_index++;
		}
		else {
			old_val_index = 0;
			avg_values = old_values[old_val_index];
		}
	}
    //min_val = -105; //*std::min_element(values.begin(),values.end());
    if(cut_min_value)
    {
        //for(auto& val:values)
         //   val -= min_val;
    }
    //max_val =-45;//* std::max_element(values.begin(),values.end());
    recalcBasicValues();
    //setMinimumSize( (int)(margin*values.size()*2),max_val+margin*5);
}


int BarGraph::valtoY(int val)
{
	//int y = (height() -20) /10;

	int h = height()- (margin * 4);
	double factor = ((double)h) /(abs(max_val-min_val));

	int y = h-(val-min_val)*factor+margin;

	return y;

}


int BarGraph::chtoX(int ch)
{
	int x = (zb? 1.6:4) *barwidth + (margin + barwidth) * (zb? ch-10:ch) ;

	return x;

}


void BarGraph::paintBargraph(QPainter &painter)
{
    QPen p(Qt::blue);
    p.setWidth(1);
	QFontMetrics fm(painter.font());

	QPen pg(QColor( 255, 255, 255, 125));
    pg.setWidth(1);
    

	int ks = 1, ke=14;

	if(zb) 
	{
		ks = 11;
		ke = 26;
	}

	

    for(size_t k=ks ; k <= ke; k++)
	{
		if(k==13 && !zb)  continue;

		int xg = chtoX(k);
		painter.setPen(pg);
		painter.drawLine(xg, 2*margin, xg, height() - 15);
		//painter.rotate(-45);
		painter.drawText(xg-10, height() -5 , "Ch-"+QString::number(k));
		//painter.rotate(45);
	}
    
	
	
	int dec = abs(max_val-min_val)/12;
	for(double k=max_val-dec ; k > min_val; k-=dec)
	{
		int ak = ceil(k/5.0)*5;
		double ystart = valtoY(ak);
		int yg = ystart;
		painter.setPen(pg);
		painter.drawLine(barwidth, yg, width()-margin, yg);
		painter.drawText(barwidth, yg , QString::number(ak));
	}


    //int y = height() - margin* 2;
   
	QPainterPath rPath;
   
	pg = QPen(QColor( 255, 255, 255, 255));

    int x_lbl = margin+barwidth/2;
    for(size_t i=0, s = values.size(); i < s; ++i)
    {
		QPolygon P = barsP[i];
		int x2 = (barwidth-5*(zb?1:4))/2;

		if (i != 0 && zb) {

			double y1 = valtoY(max_values[i]) + margin;
			double y2 = valtoY(max_values[i-1]) + margin;

			painter.setPen(QPen(QColor(255, 0, 0, 255), 2));
			painter.drawLine(QPoint(chtoX(channels[i]), y1), QPoint(chtoX(channels[i - 1]), y2));
			

			y1 = valtoY(avg_values[i]) + margin;
			y2 = valtoY(avg_values[i - 1]) + margin;

			painter.setPen(QPen(QColor(0, 0, 255, 255), 2));
			painter.drawLine(QPoint(chtoX(channels[i]), y1), QPoint(chtoX(channels[i - 1]), y2));
			
			painter.setPen(pg); 


			//painter.drawLine(QPoint(chtoX(channels[i]), P.point(0).y()), QPoint(chtoX(channels[i - 1]), barsP[i - 1].point(0).y()));
		}

		rPath.lineTo(P.point(0));
		QColor col = ColHash[label[i]];//cols[i];
		painter.setPen(col);
        painter.setBrush(col);
		//painter.drawRect(bars[i]);
		painter.drawPolygon(P);

		pg.setWidth(4);
		painter.setPen(pg);
		
		QString lbl2=getLabel(i);//+" "+QString::number(values[i]);
		if(!zb)painter.drawText((P.point(0).x()+P.point(1).x())/2-fm.width(lbl2)/2, P.point(0).y() , lbl2);

		/*
        painter.rotate(90);
        if(show_label)
            painter.drawText(bars[i].left(), bars[i].top(), getLabel(i));

        int min = cut_min_value ? min_val : 0;
        QString lbl = "("+QString::number(values[i]+min)+")";
        painter.drawText(x_lbl-fm.width(lbl)/2,y+fm.height(),lbl);
        x_lbl += margin+barwidth;
        painter.rotate(-90);*/
    }

	rPath.closeSubpath();
	//painter.drawPath(rPath);

	//painter.drawPath(points);
	
    //if(!ylabel.isEmpty())
    {//-fm.width(ylabel)/2

        //painter.drawText(height()/2,width()/2,"ylabel "+ylabel);
    }

	QString title = zb ? "ZigBee Energy Level" : "WiFi RSSI";


	painter.setPen(pg);
	painter.drawText(width()/2-fm.width(ylabel)/2, barwidth/2, title);

	int angle = 90;
	painter.rotate(-angle);
	
	painter.translate(-height(), 0.0);
	painter.drawText(height()/2-fm.width(ylabel)/2 , barwidth/2,  ylabel);
	painter.rotate(angle);


	
}

void BarGraph::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	
    paintBargraph(painter);
}

void BarGraph::mouseMoveEvent(QMouseEvent *event)
{
    if(!show_tooltip)
        return;
    QPoint pos = event->pos();
    auto it = std::find_if(bars.begin(),bars.end(),[pos](const QRect& r){return r.contains(pos);});
    if(it != bars.end())
    {
        auto i = std::distance(bars.begin(),it);
        setToolTip(getLabel(i));
    }
    else if(!toolTip().isEmpty())
        setToolTip("");
}

void BarGraph::resizeEvent(QResizeEvent *event)
{
    recalcBasicValues();
}

void BarGraph::recalcBasicValues()
{
    double bF = 4;
	barwidth = std::max(margin, (int)((width()-(margin*16))/18));

    int h = height()- (margin * 4);

    double factor = ((double)h) /(abs(max_val)-abs(min_val));

    //if(min_val < 0)
    //    h -= min_val;
	if(zb) bF = 1;
	
    if(bars.size() != values.size())
	{
		bars.resize(values.size());
		barsP.resize(values.size());
	}
    int x = margin, y;

    for(size_t i=0, s = values.size(); i < s; ++i)
    {
        double ystart = valtoY(values[i]);

		//int xg = (zb? 1:4) *barwidth + (margin + barwidth) * (zb? k-10:k);

		x = chtoX(channels[i])-(zb? 0.5:2)*(margin + barwidth);
		y = ystart+margin;

        //bars[i].setRect(x, y, 4*(margin + barwidth), );
        barsP[i] = QPolygon(4);
		barsP[i].setPoint(0, QPoint(x+5*bF,y));
		barsP[i].setPoint(1, QPoint(x+bF*(margin + barwidth)-5*bF,y));
		
		barsP[i].setPoint(2, QPoint(x+bF*(margin + barwidth), valtoY(min_val)+margin));
		barsP[i].setPoint(3, QPoint(x, valtoY(min_val)+margin));

		if(ColHash.contains(label[i]) == 0)ColHash[label[i]] = QColor(qrand() % 255, qrand() % 255, qrand() % 255, 0x50);
    }
}

QString BarGraph::getLabel(size_t i)
{
    if(label.size() > i)
        return label[i];
    if(values.size() > i)
    {
        if(cut_min_value)
            return QString::number(values[i]+min_val);
        return QString::number(values[i]);
    }
    return QString::number(i);
}
