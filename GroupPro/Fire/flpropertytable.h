/***************************************************************************************
*                                                                                      *
*	   **************************************************************************      *
*	   **            Company Confidential - For Internal Use Only              **      *
*	   **                   Mircom Group of Companies("MGC")     	           **      *
*	   **                                                                      **      *
*	   **   This information is confidential  and the exclusive property of    **      *
*	   ** Mircom.  It is intended for internal use and only for the purposes   **      *
*	   **   provided,  and may not be disclosed to any third party without     **      *
*	   **                prior written permission from Mircom.                 **      *
*	   **                                                                      **      *
*	   **                          Copyright 2016                              **      *
*	   **                            Rong Huang(ron)                           **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/
#ifndef FLPROPERTYTABLE_H
#define FLPROPERTYTABLE_H

#include <QTableWidget>
#include <QTreeWidgetItem>
#include "flobject.h"

class FLPropertyTable : public QTableWidget
{
	Q_OBJECT

public:
	FLPropertyTable(QWidget *parent);
	~FLPropertyTable();
	void setNode( FLObject *node);
private:
	void SetJobNode(FLObject* node);
	void SetFloorNode( FLObject *node);
	void SetZCNode(FLObject* node);

	FLObject* flobject_node;
signals:
	void PropertyChanged(FLObject* node,QString strKey, QString strValue);
protected slots:
	void editChanged(const QString &);
	void comboChanged(const QString &);

	void on_edit_finished();

	void on_setcorrs();
	void on_setff();
};

#endif // FLPROPERTYTABLE_H
