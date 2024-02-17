#ifndef MYSTYLEDELEGATE_H
#define MYSTYLEDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QStyledItemDelegate>

class MyStyleDelegate : public QItemDelegate
{
	 Q_OBJECT
public:
	MyStyleDelegate(QObject *parent = 0);

	 QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
	 void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};


class HtmlItemDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	HtmlItemDelegate(QObject *parent = 0);

	QString anchorAt(QString html, const QPoint &point) const;

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // MYSTYLEDELEGATE_H

