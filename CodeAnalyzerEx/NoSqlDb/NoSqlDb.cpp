/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Karthik Palepally Muniyappa, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////
#include "NoSqlDb.h"
#include <iostream>
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
#ifdef DEBUGNOSQLDB

int main()
{
	std::cout << "\n  Demonstrating NoSql Helper Code";
	std::cout << "\n =================================\n";
	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";
	std::vector<std::string> vector;
	vector.push_back("cjbdcj");
	vector.push_back("bshbjsb");
	NoSqlDb<StrData> db;
	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.children = vector;
	elem1.data = "elem1's StrData";
	db.save(elem1.name, elem1);
	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.data = "elem2's StrData";
	db.save(elem2.name, elem2);
	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.data = "elem3's StrData";
	db.save(elem3.name, elem3);
	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";
	std::cout << "\n  Creating and saving NoSqlDb elements with int data";
	std::cout << "\n ----------------------------------------------------\n";
	NoSqlDb<intData> idb;
	Element<intData> ielem1;
	ielem1.name = "elem1";
	ielem1.category = "test";
	ielem1.data = 1;
	idb.save(ielem1.name, ielem1);
	
}
#endif