#pragma once
///////////////////////////////////////////////////////////////////////////////////////
// TypeTable.h - stores file names, type names ,namesspace for each type defined     //
//                                                                                    //
// Karthik Palepally Muniyappa,CSE687 - Object Oriented Design, Spring 2017          // 
//                                                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines an TypeInfo Class which is a container class for type table and it also defines TypeTable class

TypeTable class defines functions to:
build dependency table based on type info stored in type table
return keys stored in the database
return row for a given key from the db
show the contents of the dependecy table

Public Interface:
=================
TypeTable();//constructor
bool checkType(ASTNode * pNode);
void storeGlobalFunction(ASTNode * pNode);
void typeAnalysis(ASTNode * pNode);
void doTypeAnal();

void showTypeTable();
std::vector<std::string> getTypeDefinationFnames(std::string typeName);
* Required Files:
* ---------------
*   - AbstrSynTree.h
*   - ActionsAndRules.h
*    
* Build Process:
* --------------
*   devenv TypeTable.sln /debug rebuild
*
* Maintenance History:
* --------------------
ver 0.1 : 7nd March 2017
*
/*
* -
*/
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include<stack>

#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../Parser/ActionsAndRules.h"

namespace CodeAnalysis
{ 
	// container class for type table
	class TypeInfo
	{
	private:
		std::string type;
		std::string file;
		std::string nameSpace;
		
	public :
		std::string getType();
		void setType(std::string tp);
		std::string getFile();
		void setFile(std::string fi);
		std::string getNamespace();
		void setNamespace(std::string nmsp);
	};
	//getter for type
	inline std::string CodeAnalysis::TypeInfo::getType()
	{
		return type;
	}
	//setter for type
	 inline void CodeAnalysis::TypeInfo::setType(std::string tp)
	{
		 type = tp;
	}
	 //getter for file
	 inline std::string CodeAnalysis::TypeInfo::getFile()
	 {
		 return file;
	 }
	 //setter for file
	 inline void CodeAnalysis::TypeInfo::setFile(std::string fi)
	 {
		 file = fi;
	 }
	 //getter for namespace
	 inline std::string CodeAnalysis::TypeInfo::getNamespace()
	 {
		 return nameSpace;
	 }
	 //setter for namespace
	 inline void CodeAnalysis::TypeInfo::setNamespace(std::string nmsp)
	 {
		 nameSpace = nmsp;
	 }
	 using Type = std::string;
	 using stringMap = std::unordered_map<std::string, std::string>;

	class TypeTable
	{
	public:
		TypeTable();
		void doTypeAnal();
		void showTypeTable();
		std::vector<std::string> getTypeDefinationFnames(std::string typeName);



	private:

		AbstrSynTree& ASTref_;
		bool doNotInclFun = false;
		std::string parentSetter = "bd";
		void TypeTable::extractGlobalFunctions(ASTNode * globalNode);
		bool globFunWchild = false;
		bool checkType(ASTNode * pNode);
		void storeGlobalFunction(ASTNode * pNode);
		void typeAnalysis(ASTNode * pNode);
		std::vector<std::string> nonGlobalFunc;
		std::unordered_map<Type, std::vector<TypeInfo>> typeTableMap;
		std::unordered_map<Type, std::vector<std::string>> globalFuncMap;
		std::stack<std::string> nameSpaceStack;
		void chekIfGlobalFun(ASTNode * pNode);
		void chekIfGlobalFunWithChild(ASTNode * pNode);
	};
}


	

	

	



