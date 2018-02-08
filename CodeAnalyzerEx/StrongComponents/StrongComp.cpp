#include"StrongComp.h"
#include"../XmlDocument/XmlDocument/XmlDocument.h"
#include"../XmlDocument/XmlElement/XmlElement.h"
using namespace CodeAnalysis;
// contructor of graph class
Graph::Graph()
{

}
//adds an edge to the graph
void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w);
}


//initializes the number of vertices of the graph
void Graph::initGraph(int siz)
{
	this->V = siz;
	adj = new list<int>[siz];

}
// returns an adjacency matric of the graph
list<int>* Graph::getAdjMatrix()
{
	return adj;
}
//contructor
StronglyCC::StronglyCC(string xmlName)
{
	std::cout << "\n***********************Requirement 3-Created Strong Component Packages**********************************\n";
	//sets the name for the scc xml file
	xmlPath = xmlName;
}

//initiates finding scc using DependencyTable obj
void StronglyCC::buildSCC(DependencyTable &objDP)
{
	std::cout << "\n****************************Requirement 6: Strongly Connected Package : Finding Strongly connected components using Dependency Table**************************************************\n";
	vector<string> files = objDP.getDBKeys();
	buildFileMap(files);
	V = static_cast<unsigned int>(files.size());
	depGraph.initGraph(static_cast<unsigned int>(files.size()));
	std::cout << "\n****************************Requirement 6: Building dependency graph from dependency table**************************************************\n";
	// populate the graph from dependency table
	for (string file : files) {
		Element<std::string> record = objDP.getRecord(file);
		std::vector<std::string> children = record.children;
		for (string child : children) {
			if (!(file == child))
			{
				depGraph.addEdge(FileToInt[file], FileToInt[child]);
			}
		}
	}
	adjMatrix = depGraph.getAdjMatrix();
	SCC();
	persistSCToXML();
	std::cout << "\n*************************SCC xml file stored in " << FileSystem::Path::getFullFileSpec(xmlPath) << " ******************************\n";
	std::cout << "\n" << xmlFileContent << "\n";
	writeTofile(xmlFileContent, xmlPath);
	std::cout << "\n***********************************************************************\n";
	std::cout << "\n****************************Requirement 7: Strongly Connected Components analysis writen to an xml file : " << FileSystem::Path::getFullFileSpec(xmlPath) << " **************************************************\n";
	//showSCC();
}
//persists scc to an xml file
bool CodeAnalysis::StronglyCC::persistSCToXML()
{
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("StronglyConnectedComponents");
	doc.docElement() = pRoot;
	
	for (auto sc : strngComps)
	{
		SPtr pElem = makeTaggedElement("SCC");
		string name = "SCC" + to_string(sc.first + 1);
		pElem->addAttrib("Name", name);
		for (string file : sc.second)
		{
			SPtr fileElem = makeTaggedElement("File");
			SPtr pfileTextElem = makeTextElement(file);
			fileElem->addChild(pfileTextElem);
			//adding data to element
			pElem->addChild(fileElem);
		}
		pRoot->addChild(pElem);
	}
	xmlFileContent = doc.toString();
	return true;
}

// builds a mapping between the file names and integers
void StronglyCC::buildFileMap(vector<string> files)
{
	int k = 0;
	for (size_t i = 0; i < files.size(); i++) {
		FileToInt[files[i]] = k;
		k++;
	}
}

//tarjans algorithm
void StronglyCC::SCC()
{
	
	int *disc = new int[V];
	int *low = new int[V];
	bool *stackMember = new bool[V];
	stack<int> *st = new stack<int>();
	// Initialize disc and low, and stackMember arrays
	for (int i = 0; i < V; i++)
	{
		disc[i] = NIL;
		low[i] = NIL;
		stackMember[i] = false;
	}
	// Call the recursive function to find strongly connected components in DFS tree with vertex 'i'
	for (int i = 0; i < V; i++)
		if (disc[i] == NIL)
			SCCUtil(i, disc, low, st, stackMember);
}

void StronglyCC::SCCUtil(int u, int disc[], int low[], stack<int>* st, bool stackMember[])
{
	static int time = 0;
	// Initialize discovery time and low value
	disc[u] = low[u] = ++time;
	st->push(u);
	stackMember[u] = true;
	// Go through all vertices adjacent to this
	list<int>::iterator i;
	for (i = adjMatrix[u].begin(); i != adjMatrix[u].end(); ++i)
	{
		int v = *i;  // v is current adjacent of 'u'
	   // If v is not visited yet, then recur for it
		if (disc[v] == -1)
		{
			SCCUtil(v, disc, low, st, stackMember);
			// Check if the subtree rooted with 'v' has a connection to one of the ancestors of 'u'
			low[u] = min(low[u], low[v]);
		}
		// Update low value of 'u' only of 'v' is still in stack,it's a back edge, not cross edge).
		else if (stackMember[v] == true)
			low[u] = min(low[u], disc[v]);
	}
	// head node found, pop the stack and print an SCC
	int w = 0;  // To store stack extracted vertices
	if (low[u] == disc[u])
	{
		while (st->top() != u)
		{
			w = (int)st->top();
			//cout << w << " ";
			comp.push_back(getFileName(w));
			stackMember[w] = false;
			st->pop();
		}
		w = (int)st->top();
		//cout << w << "\n";
		comp.push_back(getFileName(w));
		strngComps[strongCompsIndex] = comp;
		strongCompsIndex++;
		comp.clear();
		stackMember[w] = false;
		st->pop();
	}
}

//remap integers to file names
string StronglyCC::getFileName(int index)
{
	for (auto i : FileToInt)
	{
		if (i.second == index)
		{
			return i.first;
		}
	}
	return string();
}
//display scc
void StronglyCC::showSCC()
{
	for (auto i : strngComps)
	{
		cout << "StrongComponent:" << (i.first + 1)<<" ";
		cout << "\n";
		for (string files : i.second)
		{
			cout << files << "   ";
			//cout << "\n";
		}
		cout << "\n";
	}
}

// write scc xml to a file
bool CodeAnalysis::StronglyCC::writeTofile(std::string content, std::string fName)
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

#ifdef DEBUGSCC
int main()
{
	DependencyTable objDP("../../b.xml")
	StronglyCC objscc("../../xmlName.xml");
	objscc.buildSCC(objDP);
	objscc.showSCC();
	return 0;
}
#endif

