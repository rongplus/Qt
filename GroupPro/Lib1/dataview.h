//////////////////////////////////////////////////////////////////////////////
// dataview.h
//
//  A data viewer with a QTextBrowser, navigate button Previous and Next to
//  display one or more piece of data.
//  It is used for displaying QC and Reason in iDataFax data tab.
//////////////////////////////////////////////////////////////////////////////

#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QWidget>
#include <QStringList>


#include <QStyledItemDelegate>

#include <QList>
#include "ui_dataview.h"
#include "lib1_global.h"



class HtmlItemDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	HtmlItemDelegate(QObject *parent = 0);

	QString anchorAt(QString html, const QPoint &point) const;

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



class LIB1SHARED_EXPORT DataView : public QWidget, private Ui::DataView
{
    Q_OBJECT
public:
    DataView(QWidget *parent = 0);

    void clear();

    void setDataName(const QString &name);

    void setData(const QString &data, const QString &tooltip=QString());
    void setDataList(const QStringList &dlist, const QList<int> &clist);
    void updateDataList(const QStringList &dlist, const QList<int> &clist);

    void setCurrentCode(int code);
    int  getCurrentCode();

    void setType(bool bQueryView);

public slots:
    void slotPrevData();
    void slotNextData();
    void slotCurrentIndexChanged(int index);
    void slotEnableReasonMenu(bool bReply, bool bEdit, bool bDel);
    void slotEnableQueryMenu(bool bAdd, bool bEdit, bool bReply, bool bDel);
	void on_btnView_clicked();

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);
    void reset(const QStringList &dlist, const QList<int> &clist, int index);
    void setEnableButtons();

signals:
    void ReplyRequest();
    void AddRequest();
    void DelRequest();
    void EditRequest();
    void CodeChanged(int code);

private:
    QStringList datalist;
    QList<int>  codelist;
    int         curr_idx;
    bool m_q_mode, m_r_mode;
    bool mr_add, mr_edit, mr_del;
    bool mq_add, mq_edit, mq_del, mq_reply;
};



#endif // DATAVIEW_H

