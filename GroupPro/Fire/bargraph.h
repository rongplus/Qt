#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>

class BarGraph : public QWidget
{
    Q_OBJECT
	std::vector<int> channels;
    std::vector<int> values;

	std::vector<int> min_values;
	std::vector<int> max_values;
	std::vector<int> avg_values;

	std::vector<int> old_values[10];
	int old_val_index;

    std::vector<QString> label;
	std::vector<QColor> cols;
	

	QHash<QString, QColor> ColHash;

    int max_val,min_val,barwidth,margin;
    std::vector<QRect> bars;
	std::vector<QPolygon> barsP;
 

    bool show_tooltip,show_label,cut_min_value;
    QBrush brush;
    QString ylabel;
	bool zb;
public:
    explicit BarGraph(QWidget *parent = 0);

	QHash<QString, QColor> getColors(void){return ColHash;};
    void setData(std::vector<int> channels, std::vector<int> values, std::vector<QString> labels=std::vector<QString>(), bool zb=false);
    void setLabel(std::vector<QString> labels){ label = std::move(labels);}
    void paintBargraph(QPainter& painter);

    void setShow_tooltip(bool value);

    bool getShow_label() const;
    void setShow_label(bool value);

    QBrush getBrush() const;
    void setBrush(const QBrush &value);

    bool getCut_min_value() const;
    void setCut_min_value(bool value);

    int getMargin() const;
    void setMargin(int value);

    QString getYlabel() const;
    void setYlabel(const QString &value);

	void set_min_value(int value);
    void set_max_value(int value);
	void set_zb(bool z);

signals:
    
public slots:
protected:

private:
    void paintEvent(QPaintEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;
    void recalcBasicValues();
	int valtoY(int val);
	int chtoX(int val);
    QString getLabel(size_t i);
};

#endif // BARGRAPH_H
