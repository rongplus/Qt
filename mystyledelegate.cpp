#include <QPainter>
#include <QHelpEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>


#include "mystyledelegate.h"

MyStyleDelegate::MyStyleDelegate(QObject *parent) :
	QItemDelegate(parent)
{

}


QWidget* MyStyleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	if (!index.isValid()) return NULL;	// ASSERTION

	QWidget* wid =  QItemDelegate::createEditor(parent, option, index);
	//int value = index.model()->data(index, Qt::EditRole).toInt();
   // QSpinBox *editor = new QSpinBox(parent);
   // editor->setMinimum(0);
   // editor->setMaximum(100);
	return wid;
}

void MyStyleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QVariant value = index.model()->data(index, Qt::EditRole);
	if( value.type() == QVariant::Color)
	{
		auto options = option;
		painter->save();

		options.text = "";
		options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter);
		painter->setBrush( QBrush(Qt::red));
		painter->fillRect(option.rect,QBrush(Qt::red));
		painter->restore();
	}

		QItemDelegate::paint(painter, option, index);
		return;



}


HtmlItemDelegate::HtmlItemDelegate(QObject *parent ):QStyledItemDelegate(parent)
{

}

QString HtmlItemDelegate::anchorAt(QString html, const QPoint &point) const {
	QTextDocument doc;
	doc.setHtml(html);

	auto textLayout = doc.documentLayout();
	Q_ASSERT(textLayout != 0);
	return textLayout->anchorAt(point);
}

void HtmlItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if(index.column()==0)
	{
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}
	auto options = option;
	initStyleOption(&options, index);

	painter->save();

	QTextDocument doc;
	doc.setHtml(options.text);

	options.text = "";
	options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter);

	painter->translate(options.rect.left(), options.rect.top());
	QRect clip(0, 0, options.rect.width(), options.rect.height());
	doc.drawContents(painter, clip);

	painter->restore();
}

QSize HtmlItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	QStyleOptionViewItem options = option;
	initStyleOption(&options, index);

	QTextDocument doc;
	doc.setHtml(options.text);
	doc.setTextWidth(options.rect.width());
	return QSize(doc.idealWidth(), doc.size().height());
}
