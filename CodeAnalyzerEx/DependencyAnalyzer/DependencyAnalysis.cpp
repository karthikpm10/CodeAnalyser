#include"DependencyAnalysis.h"
using namespace CodeAnalysis;

//constructor which accepts the xml name for the dependency table xml
DependencyTable::DependencyTable(std::string xmlName)
{
	std::cout << "\n****************************Requirement 5 and 3: Dependency Analysis Package : Object created**************************************************\n";
	xmlPath =  xmlName;
	//setting the xml path for the db to persist and the write counter after which the db persists
	DependecyTableDB.setPersistCountXMlPath(xmlPath, 20);

}

//uextracts file names from file path
std::stack<std::string> DependencyTable::split(const std::string s, char delim) {
	std::stack<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (item.length() > 0) {
			elems.push(item);
		}
	}
	return elems;
}
//inserts dependency for a given key(file) into the DB
void DependencyTable::insertFileDpndncy(std::string key, std::vector<std::string> depFile)
{
	//gets the record from the db
	Element<std::string> record = DependecyTableDB.getRecord(DependecyTableDB.getDBKeys(), key);
	if (string(record.name) != "")
	{
		//iterate over depfile
		std::vector<std::string> children = record.children;
		for (size_t i = 0; i < depFile.size(); i++) {
			// chek if depFile[i] is already present in the children vector, if so dont update, if not update
			bool duplicate = false;
			for (size_t k = 0; k < children.size(); k++) {
				if ((children[k] == depFile[i]))
				{
					duplicate = true;
					break;
				}
			}
			if ((!duplicate))
				DependecyTableDB.updateElement(key, "children", depFile[i], true);
		}
	}
	else
	{
		DependecyTableDB.insertElement(key, "DependencyTable", "Files", "DependencyTableFiles", depFile, "DependentFiles");
	}

}
//builds the dependency table from the type table info and by tokenizing each file
bool DependencyTable::buildDependencyTable(std::vector<std::string> fileNames, TypeTable &typeTable)
{
	std::string fileSpec = "";
	try
	{
		std::vector<std::string>::iterator fileSpec;
		for (fileSpec = fileNames.begin(); fileSpec < fileNames.end(); fileSpec++) {

			std::stack<std::string> fileNamesSplit = split(*fileSpec, '\\');
			std::string fileName = fileNamesSplit.top();

			std::ifstream in(*fileSpec);
			if (!in.good())
			{
				std::cout << "\n  can't open " << *fileSpec << "\n\n";
				return 1;
			}
			{
				Scanner::Toker toker;
				toker.returnComments();
				toker.attach(&in);
				do
				{
					std::string tok = toker.getTok();
					if (tok == "\n")
						tok = "newline";
					//std::cout << "\n -- " << tok;
					std::vector<std::string> files = typeTable.getTypeDefinationFnames(tok);
					if (files.size() > 0)
					{
						insertFileDpndncy(*fileSpec, files);
						
					}
				} while (in.good());
			}
		}
	}
	catch (std::logic_error& ex)
	{
		std::cout << ex.what();
	}
	return 0;
}
//return the keys from the db
vector<string> DependencyTable::getDBKeys()
{
	vector<string> keyss = DependecyTableDB.getDBKeys();
	return keyss;
}
//return a record from the db
Element<std::string> DependencyTable::getRecord(std::string ky)
{
	Element<std::string> record = DependecyTableDB.getRecord(DependecyTableDB.getDBKeys(), ky);
	return record;
}
//prints the dependendcy table
void CodeAnalysis::DependencyTable::showDepTable()
{
	DependecyTableDB.showDB();
	cout << "\n****************************Requirement 7:Dependency Table Stored in " << FileSystem::Path::getFullFileSpec(xmlPath) << " *************************************\n";
	

}

#ifdef DEBUGDEPENDENCYTABLE
int main()
{
	TypeTable objtypeTable;
	objtypeTable.doTypeAnal();
	DependencyTable obj("../../xml.xml");
	obj.buildDependencyTable(fileNames, objtypeTable);
	vector<string> keys = obj.getDBKeys();
	Element<std::string> ele = obj.getRecord("A");
	showDepTable();
	return 0;
}
#endif




