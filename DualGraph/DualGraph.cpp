#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

#include "DualGraph.h"

//////////////////////////////////////////////////////////////////////////
// GraphNode
//////////////////////////////////////////////////////////////////////////

GraphNode::GraphNode(int id) 
: m_id(id) 
{

}

GraphNode::~GraphNode() {

}

void GraphNode::print() {
	std::cout << "v " << id() << "\n";
}

//////////////////////////////////////////////////////////////////////////
// GraphEdge
//////////////////////////////////////////////////////////////////////////

GraphEdge::GraphEdge(GraphNode *n1, GraphNode *n2) 
: m_from (n1), m_to(n2) 
{

}

GraphEdge::~GraphEdge() {

}

void GraphEdge::print() {
	std::cout << "e " << m_from->id() << " -- " << m_to->id() << "\n";
}

//////////////////////////////////////////////////////////////////////////
// DualGraph
//////////////////////////////////////////////////////////////////////////
DualGraph::DualGraph(void)
{
}


DualGraph::~DualGraph(void)
{
	reset();
}

GraphEdge *DualGraph::getEdge(int nid0, int nid1) {
    std::pair <int, int> p = std::make_pair(std::min(nid0, nid1), std::max(nid0, nid1));
    if (m_edges.find(p) == m_edges.end()) {
        std::cout << "no edge found! " << nid0 << " -- " << nid1 << "\n";
		return NULL;
	}
    return m_edges[p];
}

GraphNode *DualGraph::addNode() {
   GraphNode *node = new GraphNode(m_nodes.size() + 1); 
   m_nodes.push_back(node); 
   return node;
}

GraphEdge *DualGraph::addEdge(GraphNode *from, GraphNode *to) {
    GraphEdge *edge = new GraphEdge(from, to);
    std::pair<int, int> p = std::make_pair(std::min(from->id(), to->id()), std::max(from->id(), to->id()));
    if (m_edges[p]) {
        std::cerr << "duplicated edge! " << from->id() << " -- " << to->id() << "\n";
        exit(-1);
	}
    m_edges[p] = edge;

    from->addEdge(edge);
    to->addEdge(edge);

    return edge;
}

void DualGraph::print() {
	for (auto n : m_nodes) {
		n->print();
	}

	for (auto &kv : m_edges) {
		kv.second->print();
	}

}

void DualGraph::saveGraph(const char *filename) {
    std::ofstream output(filename);
    for (NodeIter nit(this); !nit.end(); ++nit) {
        GraphNode *node = *nit;
		output << "v " << node->id() << "\n";
	}

    for (EdgeIter eit(this); !eit.end(); ++eit) {
        GraphEdge *edge = *eit;
        output << "e " << edge->from()->id() << " " << edge->to()->id() << "\n";
	}
    output.close();
}

void DualGraph::loadGraph(const char *filename) {
	reset();
   std::ifstream input(filename); 
    
   std::string line;
   while (input.good()) {
	   getline(input, line);
	   if (line.empty()) {
		   continue;
	   }
	   std::stringstream ss(line);
	   std::string title;
	   ss >> title;
	   if (title == "v") {
        this->addNode(); 
	   }
	   else if (title == "e") {
		   int nid0 = 0, nid1 = 0;
		   ss >> nid0 >> nid1;
		   if (nid0 <= 0 || nid1 <= 0) {
			   std::cerr << "node id error: non-positive id!\n";
			   exit(-1);
		   }
		   this->addEdge(this->getNode(nid0), this->getNode(nid1));
	   }
   }

   input.close();
}

void sort_3(int* array) {
	if (array[0] > array[1]) std::swap(array[0], array[1]);
	if (array[1] > array[2]) std::swap(array[1], array[2]);
	if (array[0] > array[1]) std::swap(array[0], array[1]);
}

void DualGraph::loadMesh(const char *filename) {
	reset();

	std::ifstream input(filename);

   std::string line;
   std::map<std::pair<int, int>, std::vector<GraphNode*> > nodemap;

   while (input.good()) {
	   getline(input, line);
	   if (line.empty()) {
		   continue;
	   }
	   std::stringstream ss(line);
	   std::string title;
	   ss >> title;

	   if (title == "Face") {
		   int fid = 0;
		   int vids[3] = { 0, 0, 0 };
		   ss >> fid >> vids[0] >> vids[1] >> vids[2];

		   if (fid != m_nodes.size() + 1) {
			   std::cerr << "please renumber input file starting from 1\n";
			   exit(-1);
		   }
		   sort_3(vids);

		   if (vids[0] <= 0) {
			   std::cerr << "face has non-negative vertex id\n";
			   std::cerr << "  " << line << "\n";
			   exit(-1);
		   }

		   GraphNode *node = this->addNode();

		   nodemap[std::make_pair(vids[0], vids[1])].push_back(node);
		   nodemap[std::make_pair(vids[1], vids[2])].push_back(node);
		   nodemap[std::make_pair(vids[0], vids[2])].push_back(node);
           
	   }
   }

   for (auto& kv : nodemap) {
	   if (kv.second.size() > 2) {
		   std::cout << "non manifold!";
		   std::cout << "  check edge" << kv.first.first << " -- " << kv.first.second << "\n";
	   }
	   for (size_t i = 1; i < kv.second.size(); ++i) {
		   this->addEdge(kv.second[i], kv.second[(i + 1) % kv.second.size()]);
	   }
   }

	input.close();

}

void DualGraph::loadTetMesh(const char *filename) {
	reset();
	std::ifstream input(filename);

   std::string line;
   std::map<std::tuple<int, int, int>, std::vector<GraphNode*> > nodemap;

   while (input.good()) {
	   getline(input, line);
	   if (line.empty()) {
		   continue;
	   }
	   std::stringstream ss(line);
	   std::string title;
	   ss >> title;

	   if (title == "Tetrahedron") {
		   int fid = 0;
		   int vids[4] = { 0, 0, 0, 0 };
		   ss >> fid >> vids[0] >> vids[1] >> vids[2] >> vids[3];

		   if (fid != m_nodes.size())  {
			   std::cerr << "please renumber input file starting from 0\n";
			   exit(-1);
		   }
		   std::sort(vids, vids + 4);

		   if (vids[0] < 0) {
			   std::cerr << "tet has negative vertex id\n";
			   std::cerr << "  " << line << "\n";
			   exit(-1);
		   }

		   GraphNode *node = this->addNode();

		   nodemap[std::make_tuple(vids[0], vids[1], vids[2])].push_back(node);
		   nodemap[std::make_tuple(vids[1], vids[2], vids[3])].push_back(node);
		   nodemap[std::make_tuple(vids[0], vids[1], vids[3])].push_back(node);
		   nodemap[std::make_tuple(vids[0], vids[2], vids[3])].push_back(node);
           
	   }
   }

   for (auto& kv : nodemap) {
	   if (kv.second.size() > 2) {
		   std::cout << "non manifold!";
		   std::cout << "  check face" << std::get<0>(kv.first) 
			   << ", " << std::get<1>(kv.first)
			   << ", " << std::get<2>(kv.first) << "\n";
	   }
	   for (size_t i = 1; i < kv.second.size(); ++i) {
		   this->addEdge(kv.second[i], kv.second[(i + 1) % kv.second.size()]);
	   }
   }

	input.close();

}

void DualGraph::reset() {
	for (auto& n : m_nodes) {
		delete n;
	}
	m_nodes.clear();

	for (auto &kv : m_edges) {
        delete kv.second;
	}
	m_edges.clear();
}

void DualGraph::saveGV(const char *filename)  {
	std::ofstream output(filename);
	output << "graph {\n";
	for (auto &kv : m_edges) {
		output << kv.first.first << " -- " << kv.first.second << "\n";
	}
	output << "}\n";
	output.close();
}



void DualGraph::saveMetis(const char *filename) {
	std::ofstream output(filename);

	output.close();
}