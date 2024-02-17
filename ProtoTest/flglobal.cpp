#include "flglobal.h"
#include <QMessageBox>

DeviceModels* FLGlobal::gDevModels = nullptr;

FLGlobal::FLGlobal(QObject *parent)
	: QObject(parent)
{
	gDevModels = new DeviceModels();
}

FLGlobal::~FLGlobal()
{

}



bool FLGlobal::VerifyInputContent(QLineEdit* edit,QWidget*parent)
{
	if(edit->text() == "")
	{
		QMessageBox::warning(parent,"Content is null!","Please input content.");
		edit->setFocus();
		return false;
	}
	return true;
}

void FLGlobal::populateDevices() {

	QString dummyText = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

	DeviceModel *d;

	d = gDevModels->add_dev();
	d->set_base_model(DEV_UNDEFINED);
	d->set_name("Undefined Model");
	d->set_type(DeviceModel_Type_INPUT);	
	d->set_category(DeviceModel_Category_UNDEFINED_CATEGORY);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_MIRCOM);
	d->set_name("Mircom");
	d->set_type(DeviceModel_Type_OUTPUT);	
	d->set_category(DeviceModel_Category_TWO_WIRE_NOTIFICATION_APPLIANCE);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_COOPER);
	d->set_name("Cooper");
	d->set_type(DeviceModel_Type_OUTPUT);
	d->set_category(DeviceModel_Category_TWO_WIRE_NOTIFICATION_APPLIANCE);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_SYSTEM_SENSOR);
	d->set_name("System Sensor");
	d->set_type(DeviceModel_Type_OUTPUT);
	d->set_category(DeviceModel_Category_TWO_WIRE_NOTIFICATION_APPLIANCE);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_GENTEX);
	d->set_name("Gentex");
	d->set_type(DeviceModel_Type_OUTPUT);
	d->set_category(DeviceModel_Category_TWO_WIRE_NOTIFICATION_APPLIANCE);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_POTTER);
	d->set_name("Potter");
	d->set_type(DeviceModel_Type_OUTPUT);
	d->set_category(DeviceModel_Category_TWO_WIRE_NOTIFICATION_APPLIANCE);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_MPD_65PK_MIRCOM_SERIES_65);
	d->set_name("MPD-65PK Mircom Series 65");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_CONVENTIONAL_SMOKE_DETECTOR);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_MPD_65P_MIRCOM_SERIES_65);
	d->set_name("MPD-65P Mircom Series 65");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_CONVENTIONAL_SMOKE_DETECTOR);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_TD_135_FIXED_TO_135_DEGREE_F_RESSETTABLE);
	d->set_name("TD-135- Fixed to 135 Degree F. Resettable");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_CONVENTIONAL_HEAT_DETECTOR);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_TD_200_FIXED_TO_200_DEGREE_F_RESSETTABLE);
	d->set_name("TD-200- Fixed to 200 Degree F. Resettable");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_CONVENTIONAL_HEAT_DETECTOR);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_MHD_65_135_MIRCOM_SERIES_65);
	d->set_name("MHD-65-135- Mircom Series 65");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_CONVENTIONAL_HEAT_DETECTOR);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_MS_401U_SINGLE_STAGE_PULL_STATION);
	d->set_name("MS-401U- Single Stage Pull Station");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_PULL_STATION);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_GL_004K_GLASS_ROD);
	d->set_name("GL-004K- Glass Rod");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_PULL_STATION);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_MS_705U_SINGLE_ACTION_PULL_STATION);
	d->set_name("MS-705U- Single Action Pull Station");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_PULL_STATION);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_MS_710U_DUAL_ACTION_PULL_STATION);
	d->set_name("MS-710U- Dual Action Pull Station");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_PULL_STATION);
	d->set_description(dummyText.toStdString());

	d = gDevModels->add_dev();
	d->set_base_model(DEV_BB_700_SERIES_700);
	d->set_name("BB-700- Series 700");
	d->set_type(DeviceModel_Type_INPUT);
	d->set_category(DeviceModel_Category_PULL_STATION);
	d->set_description(dummyText.toStdString());
}