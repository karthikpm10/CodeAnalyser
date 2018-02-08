#pragma once
//////////////////////////////////////////////////////////////////////////////
// DBWrapper.h - key/value pair in-memory database                          //
//                                                                          //
// Karthik Palepally Muniyappa,CSE687 - Object Oriented Design, Spring 2017 // 
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines an DBWrapper Class
DBWrapper class defines functions to:
store an element in the memory map
delete an element in the memory map
update an element for an existing key
show the element of a key
show the contents of the unordered map
persist the contents of the db to an xml file
load the db from the xml file
query the db based on various parameters
Public Interface:
=================
using Key = std::string;
using Keys = std::vector<Key>;
bool insertElement(string key, string typeName, string category, string desc, vector<string> children, Data dataValue);//insert an element into the db
bool removeElement(string key);//remove an element from the db
void showDB();// show the contents of the db
bool updateElement(string key, string paramName, string paramValue,bool add=true);// update the db element
bool updateElement(string key, Data paramValue);//update the element
Element<Data> getRecord(vector<string> Keys, string key);//get the element of a specified key
vector<string> getDBKeys();//get all the db keys
vector<string> getChildren(vector<string> Keys, string key);// get the children of a specified key
vector<string> query(vector<string> Keys, string regEX, string paramName="default");// query the db and return the keys
vector<string> unionKeys(vector<string> keys1, vector<string> keys2);//return the union of two key set
vector<string> queryTimeInterval(vector<string> Keys, string startTime, string endTime = "default");// query based on time and date the element was return in the db
bool LoadDBfromXmlStr(std::string xmlFpath);//load the db from the xml file
DBWrapper(string xmlFPath,int persistCounter=20)//constructor which accepts the xml file path and the counter after which the db contents will be persisted
* Required Files:
* ---------------
*   - NoSqlDb.h
*   - Convert.h
* Build Process:
* --------------
*   devenv DBWrapper.sln /debug rebuild
*
* Maintenance History:
* --------------------
ver 0.1 : 2nd Feb 2017
*
/*
* -
*/
#include "../NoSqlDb/NoSqlDb.h"
#include<algorithm>
#include<regex>
#include <time.h>
using namespace std;
template<typename Data>
class DBWrapper
{
private:
	NoSqlDb<Data> db;
	Element<Data> updateChild(Element<Data> element, string child, bool add);
public:
	bool insertElement(string key, string typeName, string category, string desc, vector<string> children, Data dataValue);
	bool removeElement(string key);
	void showDB();
	bool updateElement(string key, string paramName, string paramValue, bool add = true);
	bool updateElement(string key, Data paramValue);
	Element<Data> getRecord(vector<string> Keys, string key);
	vector<string> getDBKeys();
	vector<string> getChildren(vector<string> Keys, string key);
	vector<string> query(vector<string> Keys, string regEX, string paramName = "default");
	vector<string> unionKeys(vector<string> keys1, vector<string> keys2);
	vector<string> queryTimeInterval(vector<string> Keys, string startTime, string endTime = "default");
	bool LoadDBfromXmlStr(std::string xmlFpath);
	bool writeDBToFile(std::string xmlFpath);
	void setPersistCountXMlPath(string xmlFPath, int persistCounter = 20);
	//accepts xmlpath of the file where the contents of the db will be wriiten and read, and a persistence counter,
	//after reaching the limit, the contents wiwll be written to file
	/*DBWrapper(string xmlFPath, int persistCounter = 20)
	{
		db.setxmlFilePath(xmlFPath);
		//cout << "\n*************************Requirement 6: Setting the Number of writes after which the db contents are persisted on line 33 of DBWrapper.h******************************\n";
		db.setPersistCounter(persistCounter);
	}*/
	DBWrapper()
	{
		std::cout << "\n***********************BONUS Requirement:Using DB cretaed by me from Project 1***************\n";
	}

};
//updates the child vector of the elements
template<typename Data>
Element<Data> DBWrapper<Data>::updateChild(Element<Data> element, string child, bool add)
{
	vector<string> childrens = element.children;
	if (add)
	{
		childrens.push_back(child);
	}
	else
	{
		childrens.erase(remove(childrens.begin(), childrens.end(), child), childrens.end());
	}
	element.children = childrens;
	return element;
}
//inserts the element into the db
template<typename Data>
bool DBWrapper<Data>::insertElement(string key, string typeName, string category, string desc, vector<string> children, Data dataValue)
{
	bool flag = false;
	try
	{
		time_t currentTime;
		Element<Data> element;
		element.name = typeName;
		element.category = category;
		element.description = desc;
		element.children = children;
		element.data = dataValue;
		element.timeDate = time(&currentTime);
		//cout << "\n**********************Requirement 3: addition of key/value pairs********************************\n";
		flag = db.save(key, element);
		//cout << "\nkey=" << key << " saved in DB\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return flag;
}
//deletes the key from the db
template<typename Data>
bool DBWrapper<Data>::removeElement(string key_delete)
{
	bool flag = false;
	try
	{
		flag = db.removeElement(key_delete);
		vector<string> keys = db.keys();
		for (string key : keys)
		{
			Element<Data> element = db.value(key);
			vector<string> childrens = element.children;
			vector<string> tempChildrens = childrens;
			//iterating over children, to update the children vector of other elements
			for (string child : childrens)
			{
				if (child == key_delete)
				{
					tempChildrens.erase(remove(tempChildrens.begin(), tempChildrens.end(), key_delete), tempChildrens.end());
				}
			}
			element.children = tempChildrens;
			db.updateElement(key, element);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return flag;
}
//displays the contents of the db
template<typename Data>
void DBWrapper<Data>::showDB()
{
	try
	{
		std::cout << "\n  Retrieving elements from NoSqlDb";
		std::cout << "\n ------------------------------------------\n";
		std::cout << "\n  size of db = " << db.count() << "\n";
		vector<string> keys = db.keys();
		for (string key : keys)
		{
			cout << "\n  " << key << ":";
			cout << db.value(key).show();
		}
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
}
//updates the element based on the paramName
template<typename Data>
bool DBWrapper<Data>::updateElement(string key, string paramName, string paramValue, bool add)
{
	try
	{
		bool updateDB = true;
		Element<Data> element = db.value(key);
		if (string(element.name) != "")
		{
			if (paramName == "name")
				element.name = paramValue;
			else if (paramName == "category")
				element.category = paramValue;
			else if (paramName == "description")
				element.description = paramValue;
			else if (paramName == "children")
				element = updateChild(element, paramValue, add);
			// else if (paramName == "parent")
				 //element.parent = paramValue;
			else
			{
				cout << "Invalid Param Name";
				updateDB = false;
				return false;
			}
			if (updateDB)
			{
				db.updateElement(key, element);
				return true;
			}
		}
		return false;
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
		return false;
	}
}
//updates the data value of the element
template<typename Data>
bool DBWrapper<Data>::updateElement(string key, Data paramValue)
{
	bool flag = false;
	try
	{
		Element<Data> element = db.value(key);
		if (string(element.name) != "")
		{
			element.data = paramValue;
			flag = db.updateElement(key, element);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return flag;
}
//gets the element of the key 
template<typename Data>
Element<Data> DBWrapper<Data>::getRecord(vector<string> Keys, string key)
{
	Element<Data> record;
	try
	{
		for (string DBKey : Keys)
		{
			if (DBKey == key)
			{
				record = db.value(key);
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return record;
}
//returnss the db keys
template<typename Data>
vector<string> DBWrapper<Data>::getDBKeys()
{
	return db.keys();
}
//returns the children of the key
template<typename Data>
vector<string> DBWrapper<Data>::getChildren(vector<string> Keys, string key)
{
	vector<string> children;
	try
	{
		regex regexp(key);
		for (string DBKey : Keys)
		{
			if (regex_match(DBKey, regexp))
			{
				children = db.value(key).children;
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return children;
}
//queries the element parameters based on the regular expression given 
template<typename Data>
vector<string> DBWrapper<Data>::query(vector<string> Keys, string regEX, string paramName)
{
	//cout << "*************************Requirement 12: Queries Implemented using Reg ex  line 316 of DBWrapper.h*******************************";
	vector<string> matchedKeys;
	try
	{
		regex regexp(regEX);
		string searchParam;
		for (string DBKey : Keys)
		{
			if (paramName == "ItemName")
				searchParam = db.value(DBKey).name;
			else if (paramName == "category")
				searchParam = db.value(DBKey).category;
			else if (paramName == "description")
				searchParam = db.value(DBKey).description;
			else if (paramName == "data")
				searchParam = Convert<Data>::toString(db.value(DBKey).data);
			else
			{
				if (paramName == "default")
				{
					if (regex_match(DBKey, regexp))
					{
						matchedKeys.push_back(DBKey);
					}
				}
			}
			if (regex_match(searchParam, regexp))
				matchedKeys.push_back(DBKey);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return matchedKeys;
}
// returns the union of two key vectors
template<typename Data>
vector<string> DBWrapper<Data>::unionKeys(vector<string> keys1, vector<string> keys2)
{
	try
	{
		for (string key1 : keys1)
		{
			bool contains = false;
			for (string key2 : keys2)
			{
				if (key1 == key2)
					contains = true;
			}
			if (!contains)
				keys2.push_back(key1);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return keys2;
}
//queries based on time date of element
template<typename Data>
vector<string> DBWrapper<Data>::queryTimeInterval(vector<string> Keys, string startTime, string endTime)
{
	vector<string> returnKeys;
	try
	{
		time_t currentTime;
		time_t startTimeobj = db.DateTimeFromString(startTime.c_str());
		time_t endTimeobj = (endTime != "default" ? db.DateTimeFromString(endTime.c_str()) : time(&currentTime));
		for (string DBKey : Keys)
		{
			time_t elemTime = db.value(DBKey).timeDate;
			if ((elemTime > startTimeobj) && (elemTime <= endTimeobj))
			{
				returnKeys.push_back(DBKey);
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return returnKeys;
}
//persists db from xml file
template<typename Data>
bool DBWrapper<Data>::LoadDBfromXmlStr(std::string xmlFpath)
{
	try
	{
		db.DBfromXmlStr(xmlFpath);
		return true;
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return false;
}
//writes the contents of db to a file
template<typename Data>
bool DBWrapper<Data>::writeDBToFile(std::string xmlFpath)
{
	try
	{
		db.DBtoXmlStr(xmlFpath);
		return true;
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return false;
}

template<typename Data>
void DBWrapper<Data>::setPersistCountXMlPath(string xmlFPath, int persistCounter)
{
	db.setxmlFilePath(xmlFPath);
	//cout << "\n*************************Requirement 6: Setting the Number of writes after which the db contents are persisted on line 33 of DBWrapper.h******************************\n";
	db.setPersistCounter(persistCounter);
}
