#include <QPainter>
#include <QHelpEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QDateEdit>
#include <QSpinBox>
#include <QtWidgets>

#include "mystyledelegate.h"

#define aaaa tr("bbb")

MyStyleDelegate::MyStyleDelegate(QObject *parent) :
	QItemDelegate(parent)
{

}


QWidget* MyStyleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	if (!index.isValid()) return NULL;	// ASSERTION

	QWidget* wid =  QItemDelegate::createEditor(parent, option, index);
	auto ee = qobject_cast<QDateEdit*>(wid);
	if(ee)
		ee->setDisplayFormat("yyyy:mm:dd");

	auto sp = qobject_cast<QSpinBox*>(wid);
	QCheckBox* ch = qobject_cast<QCheckBox*> (wid);
	auto cb = qobject_cast<QComboBox*>(wid);

	if(cb)
	{
		cb->clear();
		cb->addItem("yes");
		cb->addItem("no");
	}
	if(ch)
	{

		ch->setStyleSheet(QLatin1String("\nbackground-color:rgb(255, 255, 255);"));
		ch->setText("Test Only");
		ch->setChecked( index.data().toBool());

	}

		/*else if( pro == DFSITE_BEGINDATE || pro == DFSITE_ENDDATE || pro == DFSITE_PROTOCOLDATE1
				 || pro == DFSITE_PROTOCOLDATE2 || pro == DFSITE_PROTOCOLDATE4
				 || pro == DFSITE_PROTOCOLDATE3 || pro == DFSITE_PROTOCOLDATE5)
		{
			if(index.data().toString() !="")
			{
				QDateEdit* de = new QDateEdit(pt);
				de->setDisplayFormat("yyyy/MM/dd");
				de ->setDate( index.data().toDate() );
				return de;
			}
		}*/

	return wid;
}


void MyStyleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
								const QModelIndex &index) const
{
	auto ee = qobject_cast<QDateEdit*>(editor);
	if(ee != nullptr){
		model->setData(index, QVariant::fromValue(ee->date().toString("yyyy/MM/dd") ));

	} else {
		auto cb = qobject_cast<QComboBox*>(editor);
		QItemDelegate::setModelData(editor, model, index);
	}
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
