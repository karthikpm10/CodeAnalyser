#include "DisplayEx.h"
using namespace CodeAnalysis;

// displays the type table
void CodeAnalysis::Disp::showTypeTable(TypeTable & objtypeTable)
{
	std::cout << "\n***************************************************************************************************************\n";
	std::cout << "\n********************Showing Type Table From DisplayEX package********************\n";
	std::cout << "\n*****************************TypeTable******************************\n";
	objtypeTable.showTypeTable();
	std::cout << "\n***************************************************************************************************************\n";
}
// displays the Dependency table table
void CodeAnalysis::Disp::showDepTable(DependencyTable & objDepTable)
{
	std::cout << "\n***************************************************************************************************************\n";
	std::cout << "\n********************Showing Dependency Table From DisplayEX package********************\n";
	std::cout << "\n*****************************Dependency Table******************************\n";
	objDepTable.showDepTable();
	std::cout << "\n***************************************************************************************************************\n";
}
// displays SCC
void CodeAnalysis::Disp::showStronglyCC(StronglyCC & objSCC)
{
	std::cout << "\n***************************************************************************************************************\n";
	std::cout << "\n********************Showing Strongly Connected Components From DisplayEX package********************\n";
	std::cout << "\n*****************************Strongly Connected Components******************************\n";
	objSCC.showSCC();
	std::cout << "\n***************************************************************************************************************\n";
}
//constructor
CodeAnalysis::Disp::Disp()
{
	std::cout << "\n**************************** Requirement  3: Display Package : Object created**************************************************\n";
}


#ifdef DISPLAY
int main()
{
	Disp objDisp;
	TypeTable objttb;
	objttb.doTypeAnal();
	DependencyTable objDepTable("../../xml1");
	objDepTable.buildDependencyTable(fileNames, objttb);
	StronglyCC objSCC("../../xml2.xml");
	objSCC.buildSCC(objDepTable);

	objDisp.showTypeTable(objttb);
	objDisp.showDepTable(objDepTable);
	objDisp.showStronglyCC(objSCC);
	

	return 0;
}
#endif // DISPLAY
