#pragma once
///////////////////////////////////////////////////////////////////////////////////////
// DisplayEx.h - Displays the type table, dependency table and SCC                   //
//                                                                                    //
// Karthik Palepally Muniyappa,CSE687 - Object Oriented Design, Spring 2017          // 
//                                                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a Disp Class
Disp class provides functions to:
show the type table
show the dependency table
show Strongly connected Components

Public Interface:
=================
void showTypeTable(TypeTable &objtypeTable);
void showDepTable(DependencyTable &objDepTable);
void showStronglyCC(StronglyCC &objSCC);
Disp();
* Required Files:
* ---------------
*   - Typtable.h
*   - DependencyAnalysis.h
*    -StrongComp.h
* Build Process:
* --------------
*   devenv DisplayEx.sln /debug rebuild
*
* Maintenance History:
* --------------------
ver 0.1 : 7nd March 2017
*
/*
* -
*/
#include"../TypeTable/Typtable.h"
#include "../DependencyAnalyzer/DependencyAnalysis.h"
#include "../StrongComponents/StrongComp.h"

namespace CodeAnalysis
{
	class Disp
	{
	public:
		void showTypeTable(TypeTable &objtypeTable);
		void showDepTable(DependencyTable &objDepTable);
		void showStronglyCC(StronglyCC &objSCC);
		Disp();
		
	};
}