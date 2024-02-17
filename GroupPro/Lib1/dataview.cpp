//////////////////////////////////////////////////////////////////////////////
// dataview.cpp
//
//  A data viewer with a QTextBrowser, navigate button Previous and Next to
//  display one or more piece of data. 
//////////////////////////////////////////////////////////////////////////////

#include <QPainter>
#include <QHelpEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QMessageBox>
#include "dataview.h"

static bool is_pressing_btn = false;

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



DataView::DataView(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    m_q_mode = m_r_mode = false;
    mr_add = mr_edit = mr_del = false;
    mq_add = mq_edit = mq_del = mq_reply = false;

    connect(addBtn,    SIGNAL(clicked()), this, SIGNAL(AddRequest()));
    connect(deleteBtn, SIGNAL(clicked()), this, SIGNAL(DelRequest()));
    connect(editBtn,   SIGNAL(clicked()), this, SIGNAL(EditRequest()));
    connect(replyBtn,  SIGNAL(clicked()), this, SIGNAL(ReplyRequest()));
    connect(comboBox,  SIGNAL(currentIndexChanged(int)),
            this,      SLOT(slotCurrentIndexChanged(int)));

    clear();
    setType(false);

    browser->installEventFilter(this);
    addBtn->installEventFilter(this);
    editBtn->installEventFilter(this);
    replyBtn->installEventFilter(this);
    deleteBtn->installEventFilter(this);

	tableWidget->setColumnWidth(0,100);
	tableWidget->setColumnWidth(1,300);
	stackedWidget->setCurrentIndex(0);
	//HtmlItemDelegate* d = new HtmlItemDelegate(this);
	//tableWidget->setItemDelegate(d);
	QMessageBox::warning(this,"text","version 5th test");

}

bool DataView::eventFilter(QObject *o, QEvent *e)
{
    if (o == browser && e->type() == QEvent::ToolTip) {
        QHelpEvent *he = static_cast<QHelpEvent *>(e);
        QTextCursor cur = browser->document()->find("Status:");
        if (!cur.isNull()) {
            QRect c_r = browser->cursorRect(cur);
            QRect t_r = QRect(0, c_r.y(),
                              browser->viewport()->width(), c_r.height());
            if (t_r.contains(he->pos())) {
                return QWidget::eventFilter(o, e); // show tool tips
            }
        }
        return true; // do not show tool tips.
    }

    if (o == addBtn && e->type() == QEvent::Paint) {
        QRectF R = addBtn->rect();
        int X = R.x() + 7;
        int Y = R.y() + 7;
        int H = R.height()- 14;
        int W = R.width() - 14;
        QPainter P(addBtn);
        QColor color = (addBtn->isEnabled() ?
                           (is_pressing_btn ? QColor(47,  69,  69)
                                            : QColor(128, 128, 128))
                                            : QColor(192, 192, 192));
        QColor icon =  (addBtn->isEnabled() ?
                           (is_pressing_btn ? QColor(0, 204, 0)
                                            : QColor(0, 179, 0))
                                            : QColor(192, 192, 192));
        if (addBtn->isEnabled() && is_pressing_btn) {
            P.fillRect(R, QColor(218, 218, 218));
        }
        P.setPen(QPen(icon, 2.0));
        P.setRenderHint(QPainter::Antialiasing);
        P.drawLine(X, Y + H / 2, X + W, Y + H / 2);
        P.drawLine(X + W / 2, Y, X + W / 2, Y + H);
        P.setPen(QPen(color, 1.5));
        P.drawRect(R);
        return true;
    }

    if (o == deleteBtn && e->type() == QEvent::Paint) {
        QRectF R = deleteBtn->rect();
        int X = R.x() + 7;
        int Y = R.y() + 7;
        int H = R.height()- 14;
        int W = R.width() - 14;
        QPainter P(deleteBtn);
        QColor color = (deleteBtn->isEnabled() ?
                              (is_pressing_btn ? QColor(47,  69,  69)
                                               : QColor(128, 128, 128))
                                               : QColor(192, 192, 192));
        QColor icon =  (deleteBtn->isEnabled() ?
                              (is_pressing_btn ? QColor(255, 77, 77)
                                               : QColor(255, 0, 0))
                                               : QColor(192, 192, 192));
        if (deleteBtn->isEnabled() && is_pressing_btn) {
            P.fillRect(R, QColor(218, 218, 218));
        }
        P.setPen(QPen(icon, 2.0));
        P.setRenderHint(QPainter::Antialiasing);
        P.drawLine(X, Y + H / 2, X + W, Y + H / 2);
        P.setPen(QPen(color, 1.5));
        P.drawRect(R);
        return true;
    }

    if (o == editBtn && e->type() == QEvent::Paint) {
        QRectF R = editBtn->rect();
        int X = R.x() + 7;
        int Y = R.y() + 7;
        int H = R.height()- 14;
        int W = R.width() - 14;
        QPainter P(editBtn);
        QColor color = (editBtn->isEnabled() ?
                            (is_pressing_btn ? QColor(47,  69,  69)
                                             : QColor(128, 128, 128))
                                             : QColor(192, 192, 192));
        QColor icon  = (editBtn->isEnabled() ?
                            (is_pressing_btn ? QColor(102, 163, 255)
                                             : QColor(51,  133, 255))
                                             : QColor(192, 192, 192));
        if (editBtn->isEnabled() && is_pressing_btn) {
            P.fillRect(R, QColor(218, 218, 218));
        }
        P.setPen(QPen(icon, 2.0));
        P.setRenderHint(QPainter::Antialiasing);
        P.drawLine(X + W / 2, Y, X + W / 2, Y + H);
        P.drawLine(X + 1, Y + 2, X + W - 1, Y + H - 2);
        P.drawLine(X + 1, Y + H - 2, X + W - 1, Y + 2);
        P.setPen(QPen(color, 1.5));
        P.drawRect(R);
        return true;
    }

    if (o == replyBtn && e->type() == QEvent::Paint) {
        QRectF R = replyBtn->rect();
        int X = R.x() + 7;
        int Y = R.y() + 7;
        int H = R.height()- 14;
        int W = R.width() - 14;
        QPainterPath P1;
        QPainterPath P2;
        QPainter P(replyBtn);
        QColor color = (replyBtn->isEnabled() ?
                             (is_pressing_btn ? QColor(47,  69,  69)
                                              : QColor(128, 128, 128))
                                              : QColor(192, 192, 192));
        QColor icon  = (replyBtn->isEnabled() ?
                             (is_pressing_btn ? QColor(255, 163, 26)
                                              : QColor(255, 153, 0))
                                              : QColor(192, 192, 192));
        if (replyBtn->isEnabled() && is_pressing_btn) {
            P.fillRect(R, QColor(218, 218, 218));
        }
        P.setRenderHint(QPainter::Antialiasing);
        P1.moveTo(X + W + 1, Y + H);
        P1.quadTo(X + W + 1, Y + 3, X + 2, Y + 2);
        P2.lineTo(X - 4, Y + 2);
        P2.lineTo(X + 2, Y - 2);
        P2.lineTo(X + 2, Y + 6);
        P2.lineTo(X - 4, Y + 2);
        P.setPen(QPen(icon, 2.0));
        P.drawPath(P1);
        P.fillPath(P2, icon);
        P.setPen(QPen(color, 1.5));
        P.drawRect(R);
        return true;
    }

    if (e->type() == QEvent::MouseButtonPress ||
        e->type() == QEvent::MouseButtonRelease) {
        if (o == addBtn) {
            is_pressing_btn = (e->type() == QEvent::MouseButtonPress);
            addBtn->update();
        }
        if (o == editBtn) {
            is_pressing_btn = (e->type() == QEvent::MouseButtonPress);
            editBtn->update();
        }
        if (o == deleteBtn) {
            is_pressing_btn = (e->type() == QEvent::MouseButtonPress);
            deleteBtn->update();
        }
        if (o == replyBtn) {
            is_pressing_btn = (e->type() == QEvent::MouseButtonPress);
            replyBtn->update();
        }
    }

    return QWidget::eventFilter(o, e);
}
    
void DataView::clear()
{
    curr_idx = 0;
    browser->clear();
    datalist.clear();
    codelist.clear();    
    comboBox->clear();
    browser->setToolTip("");
    addBtn->setEnabled(false);
    deleteBtn->setEnabled(false);
    editBtn->setEnabled(false);
    replyBtn->setEnabled(false);
	tableWidget->setRowCount(0);
}

void DataView::setType(bool bQueryView)
{
    comboBox->setVisible(bQueryView);
    replyBtn->setVisible(bQueryView);
	btnView->setVisible(bQueryView);
}

void DataView::setDataName(const QString &name)
{
    dataLbl->setText(name);
    addBtn->setToolTip(tr("Add %1").arg(name));
    deleteBtn->setToolTip(tr("Delete %1").arg(name));
    editBtn->setToolTip(tr("Edit %1").arg(name));
}

void DataView::setData(const QString &data, const QString &tooltip)
{
    QList<int> clist;

    clear();
    if (!data.isEmpty()) {
        setDataList(QStringList(data), clist);
    }
    browser->setToolTip(tooltip);
    emit CodeChanged(0);

    setEnableButtons();
}

void DataView::reset(const QStringList &dlist,const QList<int> &clist,int index)
{    
    clear();
    datalist = dlist;
    codelist = clist;
    curr_idx = index;

    if (datalist.count() > 0) {
        setCurrentCode(curr_idx);
    }

	int row = 0;
	QString value;

    foreach (QString str, datalist) {
        int pos1 = str.indexOf("</b>")+4;
        int pos2 = str.indexOf("<br>");
		value = str.mid(pos1, pos2-pos1);
		comboBox->addItem(value);

		//initial table view
		//browser->setHtml(datalist.at(curr_idx));
		//str = str.replace("<br>","\n");
		tableWidget->insertRow(row);
		tableWidget->setItem(row,0,new QTableWidgetItem(value));
		tableWidget->setItem(row,1,new QTableWidgetItem(str));

    }

    setEnableButtons();
}

void DataView::setDataList(const QStringList &dlist, const QList<int> &clist)
{
    int prob = 0;
    int prev_idx = 0;

    if (datalist == dlist && codelist == clist) {
        if (codelist.count() > 0 && curr_idx < codelist.count()) {
            prob = codelist.at(curr_idx);
        }
        emit CodeChanged(prob);
        return;
    }
    if (codelist == clist && curr_idx < clist.count()) {
        prev_idx = curr_idx; // Keep current code unchanged.
    }

    reset(dlist, clist, prev_idx);
}

void DataView::updateDataList(const QStringList &dlist, const QList<int> &clist)
{
    int c_idx = curr_idx;

    if (clist.count() < codelist.count()) {
        if (c_idx >= clist.count()) {
            c_idx = clist.count()-1;
        } else if (c_idx < 0) {
            c_idx = 0;
        }
    } else if (clist.count() > codelist.count()) { // Make new code current.
        for (int i = 0; i < clist.count(); ++i) {
            if (codelist.indexOf(clist.at(i)) < 0) {
                c_idx = i; break;
            }
        }
    }

    reset(dlist, clist, c_idx);
}

void DataView::slotPrevData()
{
    setCurrentCode(curr_idx == 0 ? codelist[0] : codelist[curr_idx - 1]);
}

void DataView::slotNextData()
{
    setCurrentCode(curr_idx == codelist.count()-1 ? codelist[curr_idx]
                                                  : codelist[curr_idx + 1]);
}

void DataView::setCurrentCode(int code)
{
    int prob = code;
    int idx = codelist.indexOf(code);
    if (idx < 0) { // The code has been deleted.
        idx = 0;
        if (codelist.count() > 0) {
            prob = codelist.at(0);            
        } else {
            prob = 0;
        }
    }
    if (idx >= 0 && idx < datalist.count()) {
        curr_idx = idx;
        browser->setHtml(datalist.at(curr_idx));    
        prob = codelist.at(curr_idx);
    }

    emit CodeChanged(prob);
}

int DataView::getCurrentCode()
{
    if (curr_idx >= 0 && curr_idx < codelist.count()) {
        return codelist.at(curr_idx);
    }
    return 0;
}

void DataView::slotCurrentIndexChanged(int index)
{
    setCurrentCode(codelist[index]);
}

void DataView::slotEnableQueryMenu(bool bAdd,bool bEdit,bool bReply,bool bDel)
{
    m_q_mode = true;
    mq_reply = bReply;
    mq_edit = bEdit;
    mq_add = bAdd;
    mq_del = bDel;

    setEnableButtons();
}

void DataView::slotEnableReasonMenu(bool bAdd, bool bEdit, bool bDel)
{
    m_r_mode = true;
    mr_edit = bEdit;
    mr_add = bAdd;
    mr_del = bDel;

    setEnableButtons();
}

void DataView::setEnableButtons()
{
    if (m_q_mode) {
        addBtn->setEnabled(mq_add);
        deleteBtn->setEnabled(mq_del);
        editBtn->setEnabled(mq_edit);
        replyBtn->setEnabled(mq_reply);
    } else if (m_r_mode) {
        addBtn->setEnabled(mr_add);
        editBtn->setEnabled(mr_edit);
        deleteBtn->setEnabled(mr_del);
    }
}

void DataView::on_btnView_clicked()
{
	stackedWidget->setCurrentIndex(stackedWidget->currentIndex()==0?1:0);
}
