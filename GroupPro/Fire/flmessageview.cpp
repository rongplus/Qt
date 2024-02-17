#include "flmessageview.h"
#include <QLabel>
#include "flobject.h"
FLMessageView::FLMessageView(QWidget *parent)
	: QScrollArea(parent)
{
	ui.setupUi(this);
	setContentsMargins(0,0,0,0);
	ui.gridLayout->setContentsMargins(0,0,0,0);
	ui.gridLayout->setSpacing( 0 );
	lblList.append("#\n1");
	lblList.append("Data\n12 34");
	lblList.append("FCS\nOK");
	ShowMessage();
}

FLMessageView::~FLMessageView()
{

}

QPair<int,int> FLMessageView::gridPosition(QWidget * widget) 
{
  auto gp = qMakePair(-1,-1);
  if (! widget->parentWidget()) return gp;
  auto layout = ui.gridLayout;
  if (! layout) return gp;
  int index = layout->indexOf(widget);
  Q_ASSERT(index >= 0);
  int rs,cs;
  layout->getItemPosition(index, &gp.first, &gp.second, &rs, &cs);
  return gp;
}

void FLMessageView::ShowMessage()
{
	QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	policy.setHeightForWidth(true);

	for (int i = 0; i < lblList.size(); ++ i)
   {   
         auto b = new QPushButton(lblList[i]);

		 b->resize(165, b->height());

		 btnList.append(b); 
		 b->setProperty("UserData", "0x00 0x01 0x000x030x05");
		 
		 //b->setSizePolicy(policy);

         connect(b, &QPushButton::clicked, [this, b]() {btnClick(b); });
		
   }
   
   showGrid();
}

void clearLayout(QLayout* layout, bool deleteWidgets = true)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

void FLMessageView::showGrid()
{
	clearLayout(ui.gridLayout, false);
	
	for (int i = 0; i < btnList.size(); ++ i)
		ui.gridLayout->addWidget(btnList[i], 0, i);
	ui.gridLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), 1, 0);
}

void FLMessageView::btnClick(QWidget *b)
{
	QPair<int,int> p = gridPosition(b);
	auto value = b->property("UserData").toString();
	if (value == "")
		return;
	auto list = value.split(" ", QString::SkipEmptyParts);
	int nPos = 1;
	if (list.count() == 1)
	{
		list = value.split("0x", QString::SkipEmptyParts);
	}
	if (list.count() == 1)
		return;
	foreach(auto v, list)
	{
		auto bd = new QPushButton("data\n" + v);
		btnList.insert(p.second + nPos, bd);
		bd->setProperty("UserData", v);
		nPos++;
		connect(bd, &QPushButton::clicked, [this, bd]() {btnClick(bd); });
	}
	//btnList.removeOne((QPushButton*)b);
	b->setProperty("UserData", "");
    showGrid();    
}