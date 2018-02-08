#pragma once
/////////////////////////////////////////////////////////////////////////
// DepAnal.h - Demonstrate how to start developing dependency analyzer //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017           //
/////////////////////////////////////////////////////////////////////////
/*
 * You need to provide all the manual and maintenance informnation
 */

#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <functional>
#include <unordered_map>

#pragma warning (disable : 4101)  // disable warning re unused variable x, below

namespace CodeAnalysis
{
  class TypeAnal
  {
  public:
    using SPtr = std::shared_ptr<ASTNode*>;
	std::string nameSpace = "Global";
	std::string nameSpacePrev = "Global";

    TypeAnal();
    void doTypeAnal();
  std::unordered_map<std::string, std::vector<std::unordered_map<std::string,std::string>>> map;
  private:
    void DFS(ASTNode* pNode);
	void DFS2(ASTNode * pNode);
    AbstrSynTree& ASTref_;
    ScopeStack<ASTNode*> scopeStack_;
    Scanner::Toker& toker_;
  };

  inline TypeAnal::TypeAnal() : 
    ASTref_(Repository::getInstance()->AST()),
    scopeStack_(Repository::getInstance()->scopeStack()),
    toker_(*(Repository::getInstance()->Toker()))
  {
    std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
  }                                              // It doesn't do anything useful for dep anal.

  inline bool doDisplay(ASTNode* pNode)
  {
    static std::string toDisplay[] = {
      "function", "lambda", "class", "struct", "enum", "alias", "typedef"
    };
    for (std::string type : toDisplay)
    {
      if (pNode->type_ == type)
        return true;
    }
    return false;
  }

  inline bool doDisplay2(ASTNode* pNode)
  {
	  static std::string toDisplay[] = {
		   "class", "struct", "enum"
	  };
	  for (std::string type : toDisplay)
	  {
		  if (pNode->type_ == type)
			  return true;
	  }
	  return false;
  }
  inline void TypeAnal::DFS(ASTNode* pNode)
  {
    static std::string path = "";
    if (pNode->path_ != path)
    {
      std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
      path = pNode->path_;
    }
	if (pNode->type_ == "namespace")
	{
		nameSpace = pNode->name_;
	}
    if (doDisplay(pNode))
    {
      std::cout << "\n  " << pNode->name_;
      std::cout << ", " << pNode->type_;
    }
    for (auto pChild : pNode->children_)
      DFS(pChild); 
  }

  inline void TypeAnal::DFS2(ASTNode* pNode)
  {
	  static std::string path = "";
	  if (pNode->path_ != path)
	  {
		  std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
		  path = pNode->path_;
	  }
	  if (pNode->type_ == "namespace")
	  {
		  nameSpace = pNode->name_;
	  }
	  if (doDisplay2(pNode))
	  {
		  std::unordered_map<std::string, std::string> mapelem;
		  mapelem["type"] = pNode->type_;
		  mapelem["file"] = pNode->package_;
		  mapelem["namespace"] = nameSpace;
		  std::vector<std::unordered_map<std::string, std::string>> v2;
		  v2.push_back(mapelem);
		  map[pNode->name_] = v2;

		  std::cout << "\n  " << pNode->name_;
		  std::cout << ", " << pNode->type_;
	  }
	  for (auto pChild : pNode->children_)
	  {
		  if (pChild->type_ == "namespace")
			  nameSpacePrev = nameSpace;
		  DFS2(pChild);
	  }
	  if (pNode->type_== "namespace") {
		  nameSpace = nameSpacePrev;
	  }		  
  }

  inline void TypeAnal::doTypeAnal()
  {
    std::cout << "\n  starting type analysis:\n";
    std::cout << "\n  scanning AST and displaying important things:";
    std::cout << "\n -----------------------------------------------";
    ASTNode* pRoot = ASTref_.root();
    DFS2(pRoot);
    std::cout << "\n\n  the rest is for you to complete\n";
  }
}