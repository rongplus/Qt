#ifndef MYDATADB_H
#define MYDATADB_H


#include <string>
#include <list>
#include <vector>
//#include <variant> C++ 17

using namespace std;
enum class DataType
{
	Color = 0,
	Title,
	Date,
	Time,
	Line,
	Pixmap,
	Url,
	Choise
	//TODO: Add more
};

struct DataObject
{
	DataType _type;
	list<string> _data;
};

enum class SharpType
{
	Line = 0,
	Circle,
	Rectangle,
	Dot,
	Polygon,
	Triangle,
	Arrow,
	Dot_Line
};


class  MyData
{
public:
	 MyData();
	 //TODO default construction
	 MyData(string name,int major, int minor);
	 ~MyData();
private:
	 string _name;
	 int _major_version;
	 int _minor_version;
	 SharpType _type;

	 list<double> _data;
};

class MyDataDB
{
public:
	MyDataDB();

	int get_count();

	static const list<MyData>& get_data();
};

#endif // MYDATADB_H
