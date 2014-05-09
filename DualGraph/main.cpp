#include <iostream>
#include <windows.h>
#include "DualGraph.h"
#include "DisjointPathsFinder.h"

void printf_color(const char* str) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << str;
	SetConsoleTextAttribute(hConsole, 7);
	return;
}

int main(int argc, char *argv[]) {
    DualGraph graph;

	GraphNode *nodes[5];
	for (int i = 0; i < 5; ++i) {
		nodes[i] = graph.addNode();
	}

	graph.addEdge(nodes[0], nodes[1]);
	graph.addEdge(nodes[1], nodes[2]);
	graph.addEdge(nodes[2], nodes[3]);
	graph.addEdge(nodes[3], nodes[4]);
	graph.addEdge(nodes[4], nodes[0]);


	system("pause");
	return 0;
}

int demo_main(int argc, char *argv[]) {
    DualGraph graph;

    //build graph
	printf_color("Build a graph with 5 vertices and 5 edges: ");
	GraphNode *nodes[5];
	for (int i = 0; i < 5; ++i) {
		nodes[i] = graph.addNode();
	}

	graph.addEdge(nodes[0], nodes[1]);
	graph.addEdge(nodes[1], nodes[2]);
	graph.addEdge(nodes[2], nodes[3]);
	graph.addEdge(nodes[3], nodes[4]);
	graph.addEdge(nodes[4], nodes[0]);

	printf_color(" [Done]\n");

    //get node by id, **starting from 1**:
	printf_color("\n\nGet node 1: \n ");
	GraphNode *node = graph.getNode(1);
	node->print();

    //get edge by two node ids
	printf_color("\n\nGet edge (1, 2): \n");
	graph.getEdge(1, 2)->print();

	printf_color("try to get a non-exist edge (1, 4):\n");
	if (graph.getEdge(1, 4)) {
		std::cerr << "Oops, there should not be an edge (1, 4)!\n";
		exit(-1);
	}

    //Iterators
    //   edges from a node
	printf_color("\n\nIterators: \n");
	printf_color("  - edges from a node: \n");
	for (GraphNode::EdgeIter eit(node); !eit.end(); ++eit) {
		GraphEdge * edge = (*eit);
		edge->print();
	}

    //   nodes in the graph
	printf_color("  - nodes in graph: \n");
	for (DualGraph::NodeIter nit(&graph); !nit.end(); ++nit) {
		GraphNode * node = (*nit);
		node->print();
	}
    

    //   edges in the graph
	printf_color("  - edges in graph: \n");
	for (DualGraph::EdgeIter eit(&graph); !eit.end(); ++eit) {
		GraphEdge * edge = (*eit);
		edge->print();
	}

	printf_color("Loading and saving\n");
    //Graphviz readable .gv format, can generate png
	graph.saveGV("build.gv");
    //our defined .graph format
	graph.saveGraph("build.graph");

    
	//////////////////////////////////////////////////////////////////////////
	//load (from .m and .tm) and save (to .gv and .graph)
	//////////////////////////////////////////////////////////////////////////
	graph.loadMesh("square1.m");
	graph.saveGV("square1.gv");
	graph.saveGraph("square1.graph");

	graph.loadMesh("square2.m");
	graph.saveGV("square2.gv");
	graph.saveGraph("square2.graph");

	graph.loadMesh("sphere.m");
	graph.saveGV("sphere.gv");
	graph.saveGraph("sphere.graph");

	graph.loadTetMesh("cube.tm");
	graph.saveGV("cube.gv");
	graph.saveGraph("cube.graph");

    return 0;     
}