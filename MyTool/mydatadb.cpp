#include "mydatadb.h"

MyDataDB::MyDataDB()
{

}

int MyDataDB::get_count() {
	return get_data().size();

}

const list<MyData>& MyDataDB::get_data() {
	static list<MyData> type {
		{"1",1,1},
		{"2",2,2},
		{"3",3,3}
	};

	return type;
}

MyData::MyData() {
	//FIXME
}

MyData::MyData(string name, int major, int minor)
{
	_name = name;
	_major_version = major;
	_minor_version = minor;

}

MyData::~MyData() {
	//BUG Jas-->do it!
}
