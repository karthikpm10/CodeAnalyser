#pragma once
///////////////////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - computes dependencies between files based on type table info//
//                                                                                    //
// Karthik Palepally Muniyappa,CSE687 - Object Oriented Design, Spring 2017          // 
//                                                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines an DDependencyTable Class
DDependencyTable class defines functions to:
build dependency table based on type info stored in type table
return keys stored in the database
return row for a given key from the db
show the contents of the dependecy table

Public Interface:
=================
DependencyTable(std::string xmlName);
bool buildDependencyTable(std::vector<std::string> fileNames, TypeTable &typeTable);
vector<string> getDBKeys();
Element<std::string> getRecord(std::string ky);
void showDepTable();
* Required Files:
* ---------------
*   - Typtable.h
*   - DBWrapper.h
*    -Tokenizer.h
* Build Process:
* --------------
*   devenv DependencyAnalyzer.sln /debug rebuild
*
* Maintenance History:
* --------------------
ver 0.1 : 7nd March 2017
*
/*
* -
*/
#include "../TypeTable/Typtable.h"
#include "../DBWrapper/DBWrapper.h"
#include "../Tokenizer/Tokenizer.h"

namespace CodeAnalysis
{

	class DependencyTable
	{
	public:

		DependencyTable(std::string xmlName);
		bool buildDependencyTable(std::vector<std::string> fileNames, TypeTable &typeTable);
		vector<string> getDBKeys();
		Element<std::string> getRecord(std::string ky);
		void showDepTable();

	private:
		DBWrapper<string> DependecyTableDB;
		std::string xmlPath;
		std::stack<std::string> DependencyTable::split(const std::string s, char delim);
		void insertFileDpndncy(std::string key, std::vector<std::string> depFile);


	};
}

 