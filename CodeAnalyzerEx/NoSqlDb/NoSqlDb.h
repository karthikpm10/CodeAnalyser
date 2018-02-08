#pragma once
/////////////////////////////////////////////////////////////////////
// NoSqlDb.h - key/value pair in-memory database                   //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       // 
//Karthik Palepally Muniyappa, Modified CSE687                     //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines an Element Class and a NoSqlDb class
Element class defines the structure of the valu for the inmemory database,
NoSqlDb class defines functions to:
store an element in the memory map
delete an element in the memory map
update an element for an existing key
show the element of a key
show the contents of the unordered map
persist the contents of the db to an xml file
load the db from the xml file
Public Interface:
=================
using Key = std::string;
using Keys = std::vector<Key>;
Keys keys();
bool save(Key key, Element<Data> elem);//insert element into db
bool removeElement(Key key);//remove element from db
bool updateElement(Key key, Element<Data> elem);//update existing element in db
Element<Data> value(Key key);//get the element for a specified key
std::string DBtoXmlStr(std::string xmlfpath);//persist db to xml file
void DBfromXmlStr(std::string xml);//load db from xml file
time_t DateTimeFromString(const char* buff);//convert string to time_t object
std::string DateTimeToString(time_t t);//convert time_t to string
size_t count();//get the size of the unordered map
bool writeTofile(std::string content, std::string fName);// write ccontents to a file
std::string readFromFile(std::string fName);//read contenets from a file
bool setPersistCounter(int persistCount);//set the counter after which the db should be persisted to an xml file
bool setxmlFilePath(std::string xmlPath);//set the xml file path
* Required Files:
* ---------------
*   - XmlElement.h, XmlElement.cpp
**   - CppProperties.h, CppProperties.cpp
*   - XmlDocument.h, XmlDocument.cpp
*   - Convert.h
* Build Process:
* --------------
*   devenv NoSqlDb.sln /debug rebuild
*
* Maintenance History:
* --------------------
ver 0.1 : 2nd Feb 2017
*
/*
 * - fixed bug in NoSqlDb::count() by replacing
 *   unordered_map<key,Value>::count() with
 *   unordered_map<key,Value>::size();
 */
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <time.h>
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../Convert/Convert.h"
 /////////////////////////////////////////////////////////////////////
 // Element class represents a data record in our NoSql database
 // - in our NoSql database that is just the value in a key/value pair
 // - it needs to store child data, something for you to implement
 //
template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using Description = std::string;
	using TimeDate = std::time_t;
	using Children = std::vector<std::string>;
	//using Parent = std::string;
	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<Description> description; //metadata
	Property<TimeDate> timeDate;    // metadata
	Property<Children> children;    //metadata
	//Property<Parent> parent;        //metadata
	Property<Data> data;            // data
	std::string show();
};
template<typename Data>
std::string Element<Data>::show()
{
	// show children when you've implemented them
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "Description" << " : " << description;
	std::vector<std::string> Childrens = children;
	for (std::string child : Childrens)
	{
		out << "\n    " << std::setw(8) << "Childrens" << " : " << child;
	}
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	out << "\n";
	return out.str();
}
/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - you will need to add query capability
//   That should probably be another class that uses NoSqlDb
//
using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;
template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	Keys keys();
	bool save(Key key, Element<Data> elem);
	bool removeElement(Key key);
	bool updateElement(Key key, Element<Data> elem);
	Element<Data> value(Key key);
	std::string DBtoXmlStr(std::string xmlfpath);
	void DBfromXmlStr(std::string xml);
	time_t DateTimeFromString(const char* buff);
	std::string DateTimeToString(time_t t);
	size_t count();
	bool writeTofile(std::string content, std::string fName);
	std::string readFromFile(std::string fName);
	bool setPersistCounter(int persistCount);
	bool setxmlFilePath(std::string xmlPath);
	NoSqlDb()
	{
		/*std::cout << "\n*******************************Requirement 2: Instantiating template class NoSqlDb*************************\n";
		std::cout << "\nwith values as name,category,description,timedate,children relationship vector and a data field****************\n";*/
	}
	~NoSqlDb()
	{
		//persisting db to xml file
		
		//std::cout << "\n*************************Requirement 6:Pesrsisting db contents from the destructor of NoSqlDb class as the db object went out of scope******************************\n";
		std::string xml = DBtoXmlStr(xmlFpath);
	}
private:
	using Item = std::pair<Key, Element<Data>>;
	int writeCounter;
	int persistLimit = 50;
	std::string xmlFpath;
	std::unordered_map<Key, Element<Data>> store;
};
//returns the keys of the unordered_map
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	try
	{
		for (Item item : store)
		{
			keys.push_back(item.first);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return keys;
}
//saves the key with element into the unordered map
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
	try
	{
		/*time_t currentTime;
		elem.timeDate =time(&currentTime);*/
		if (store.find(key) != store.end())
		{
			return false;
		}
		store[key] = elem;
		writeCounter++;
		//checking if the db has to be persisted to an xml file
		if (writeCounter > persistLimit)
		{
			//std::cout << "\n*************************Requirement 6: Pesrsisting db contents as the number of writes exceeded the limit set******************************\n";
			writeCounter = 0;
			//writing contents of db to xml file
			DBtoXmlStr(xmlFpath);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
		return false;
	}
	return true;
}
//removes the key from the unordered map
template<typename Data>
bool NoSqlDb<Data>::removeElement(Key key)
{
	try
	{//check if the key exists
		if (store.find(key) != store.end())
		{
			store.erase(key);
			return true;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
		return false;
	}
	return false;
}
//updates a key with a new element in the unordered map
template<typename Data>
bool NoSqlDb<Data>::updateElement(Key key, Element<Data> elem)
{
	try
	{
		//check if the key exists
		if (store.find(key) != store.end())
		{
			store[key] = elem;
			return true;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
		return false;
	}
	return false;
}
// returns the value of the key from the map
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	try
	{
		if (store.find(key) != store.end())
			return store[key];
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return Element<Data>();
}
//persists the contents of the db to an xml file specified by xmlpath
template<typename Data>
std::string NoSqlDb<Data>::DBtoXmlStr(std::string xmlpath)
{ 
	std::string xml;
	try
	{
		std::cout << "\n************************* Writing the db to xml file******************************\n";
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("DB");
		doc.docElement() = pRoot;
		std::vector<std::string> ke = keys();
		for (std::string key : ke)
		{
			Element<Data> element = value(key);
			SPtr pElem = makeTaggedElement("Element");
			pElem->addAttrib("Key", key);
			pElem->addAttrib("name", element.name);
			pElem->addAttrib("category", element.category);
			pElem->addAttrib("description", element.description);
			//pElem->addAttrib("parent", element.parent);
			pElem->addAttrib("timeDate", DateTimeToString(element.timeDate));
			SPtr pDataElem = makeTaggedElement("Data");
			SPtr pDataTextElem = makeTextElement(Convert<Data>::toString(element.data));
			pDataElem->addChild(pDataTextElem);
			//adding data to element
			pElem->addChild(pDataElem);
			SPtr pChildrenElem = makeTaggedElement("Children");
			std::vector<std::string> childrens = element.children;
			//iterating over children vector
			for (std::string child : childrens)
			{
				SPtr pChildElem = makeTaggedElement("Child");
				SPtr pchildTextElem = makeTextElement(child);
				pChildElem->addChild(pchildTextElem);
				pChildrenElem->addChild(pChildElem);
			}
			pElem->addChild(pChildrenElem);
			pRoot->addChild(pElem);
		}
		xml = doc.toString();
		std::cout << "\n************************* Dependency xml file******************************\n" << "\n" << xml << "\n";
		writeTofile(xml, xmlpath);
		return xml;
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return xml;
}
//loading the db from the xml file
template<typename Data>
void NoSqlDb<Data>::DBfromXmlStr(std::string xmlFpath)
{
	try
	{
		//std::cout << "\n*************************Requirement 5: Loading the db from xml file******************************\n";
		std::string xml = readFromFile(xmlFpath);
		XmlDocument doc(xml, XmlDocument::str);
		std::vector<SPtr> elements = doc.descendents("Element").select();
		for (SPtr ele : elements)
		{
			Element<Data> element;
			std::string k = ele->getAttrib("Key");
			element.category = ele->getAttrib("category");
			element.description = ele->getAttrib("description");
			element.name = ele->getAttrib("name");
			// element.parent = ele->getAttrib("parent");
			element.timeDate = DateTimeFromString(ele->getAttrib("timeDate").c_str());
			std::string dn = ele->children()[0]->children()[0]->value();
			element.data = Convert<Data>::fromString(ele->children()[0]->children()[0]->value());
			std::vector<std::string> childrenVector;
			std::vector<SPtr> childrens = ele->children()[1]->children();
			for (SPtr child : childrens)
			{
				childrenVector.push_back(child->children()[0]->value());
			}
			element.children = childrenVector;
			save(k, element);//check if the key exists and then push into db
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
}
//converting strng date to time_t object
template<typename Data>
time_t NoSqlDb<Data>::DateTimeFromString(const char * buff)
{
	time_t timeobj;
	try
	{
		struct tm time = { 0 };
		int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
		sscanf_s(buff, "%d-%d-%d_%d:%d:%d", &year, &month, &day, &hour, &min, &sec);
		time.tm_year = year - 1900;
		time.tm_mon = month - 1;
		time.tm_mday = day;
		time.tm_hour = hour;
		time.tm_min = min;
		time.tm_sec = sec;
		timeobj = mktime(&time);
		return timeobj;
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return timeobj;
}
//converting time_t to string date
template<typename Data>
std::string NoSqlDb<Data>::DateTimeToString(time_t t)
{
	std::string dat;
	try
	{
		struct tm  timestruct = {};
		char buffer[80];
		localtime_s(&timestruct, &t);
		strftime(buffer, sizeof(buffer), "%Y-%m-%d_%X", &timestruct);
		std::string date(buffer);
		return date;
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return  dat;
}
//return the number of elements in the db
template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}
// writes content to file
template<typename Data>
bool NoSqlDb<Data>::writeTofile(std::string content, std::string fName)
{
	try
	{
		std::ofstream fstream(fName);
		if (fstream.is_open())
		{
			fstream << content;
			fstream.close();
		}
		else
		{
			std::cout << "Unable to open file";
			return false;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
		return false;
	}
	return true;
}
//reads contents from a file
template<typename Data>
std::string NoSqlDb<Data>::readFromFile(std::string fName)
{
	std::string filestr;
	try
	{
		std::string line;
		std::ifstream file(fName);
		if (file.is_open())
		{
			while (getline(file, line))
			{
				filestr = filestr + line;
				// cout << line << '\n';
			}
			file.close();
			std::cout << filestr;
		}
		else
		{
			std::cout << "Unable to open file";
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception " << ex.what();
	}
	return filestr;
}
// sets the persistance count, after which the db should be written to xml file
template<typename Data>
inline bool NoSqlDb<Data>::setPersistCounter(int persistCount)
{
	persistLimit = persistCount;
	return true;
}
//sets the xml file path where the db contents will be persisted
template<typename Data>
inline bool NoSqlDb<Data>::setxmlFilePath(std::string xmlPath)
{
	xmlFpath = xmlPath;
	return true;
}
