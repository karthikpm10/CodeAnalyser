#pragma once

///////////////////////////////////////////////////////////////////////////////////////
// StrongComp.h - computes Strongly Connected Components for files based on Dependency//
// Table Infoinfo                                                                     //
// Karthik Palepally Muniyappa,CSE687 - Object Oriented Design, Spring 2017          // 
//                                                                                  //
////////////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines an Graph Class which and  StronglyCC class

Graph class defines functions to 
add an edge
to intilize the number of vertices of the graph
and to return the adjacency matric of the graph

StronglyCC class defines functions to:
to build strongly connected components
to persist the results of SCC to an xml file
display the contents of the scc

Public Interface:
=================
Graph();
void addEdge(int v, int w);// function to add an edge to graphs
void initGraph(int siz);
list<int>* getAdjMatrix();

StronglyCC(string xmlName);
void buildSCC(DependencyTable &objDP);
bool persistSCToXML();
void SCC();// computes strongly connected components
void showSCC();

void showTypeTable();
std::vector<std::string> getTypeDefinationFnames(std::string typeName);
* Required Files:
* ---------------
*   - DependencyAnalysis.h
*    -XmlDocument.h
*    -XmlElement.h
*  
*
* Build Process:
* --------------
*   devenv StrongComponents.sln /debug rebuild
*
* Maintenance History:
* --------------------
ver 0.1 : 7nd March 2017
*
/*
* -
*/
#include"../DependencyAnalyzer/DependencyAnalysis.h"
namespace CodeAnalysis
{
#define NIL -1
	class Graph
	{
		int V;// No. of vertices
		list<int> *adj;//dynamic array of adjacency lists
	public:
		Graph();
		void addEdge(int v, int w);// function to add an edge to graphs
		void initGraph(int siz);
		list<int>* getAdjMatrix();
	};


	class StronglyCC
	{
	public:
		StronglyCC(string xmlName);
		void buildSCC(DependencyTable &objDP);
		void showSCC();
	private:
		bool persistSCToXML();
		string xmlPath = "../../XMLInputFiles/";
		string xmlFileContent;
		Graph depGraph;
		int strongCompsIndex = 0;
		unordered_map<std::string, int> FileToInt;
		int V;// No. of vertices
		list<int> *adjMatrix;
		void SCC();
		void buildFileMap(vector<string> files);
		void SCCUtil(int u, int disc[], int low[],
			stack<int> *st, bool stackMember[]);
		string getFileName(int index);
		unordered_map<int, vector<string>> strngComps;
		vector<string> comp;
		bool writeTofile(std::string content, std::string fName);

	};
}





 