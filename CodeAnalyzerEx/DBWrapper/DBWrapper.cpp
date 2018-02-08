#include "DBWrapper.h"
#include <iostream>
using namespace std;

#ifdef DEBUGDBWRAPPER
int main()
{
	vector<string> childrenvector;
	childrenvector.push_back("key2");
	childrenvector.push_back("bcsjbb");
	DBWrapper<string> obj;
	obj.setPersistCountXMlPath("../XMLInputFiles/xml1.xml", 20);
	obj.LoadDBfromXmlStr("../XMLInputFiles/xml1.xml");
	obj.insertElement("key1", "string", "cat1", "demo", childrenvector, "data1 bedhbeuj");
	childrenvector.pop_back();
	childrenvector.push_back("keydummy");
	obj.insertElement("key2", "string", "cat1", "demo", childrenvector, "data1");
	obj.insertElement("key3", "string", "cat1", "demo", childrenvector, "data1");
	obj.insertElement("key4", "string", "cat1", "demo", childrenvector, "data1");
	obj.showDB();
	obj.updateElement("key4", "children", "keydummy", false);
	obj.showDB();
	obj.updateElement("key4", "data2");
	obj.showDB();
	obj.removeElement("key2");
	std::cout << "\n  Retrieving elements after delete NoSqlDb<string>";
	obj.showDB();
	obj.query(obj.getDBKeys(), ".*ke.*");
	obj.queryTimeInterval(obj.getDBKeys(), "2017-02-07_00:17:06");
	//
}
#endif