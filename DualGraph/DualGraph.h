#pragma once
#include <vector>
#include <map>

class GraphEdge;

class GraphNode {
public:
    GraphNode(int id);
    ~GraphNode();
    int  id() const { return m_id; }
    void addEdge(GraphEdge* e) { m_edges.push_back(e); }

    //print
	void print();

    //iterator
    struct EdgeIter {
        EdgeIter(GraphNode *node) : m_it(node->m_edges.begin()), m_end(node->m_edges.end()) {}
        void operator++() { ++m_it; }
        bool end() { return m_it == m_end; }
        GraphEdge* operator*() { return *m_it; }
        std::vector<GraphEdge*>::iterator m_it, m_end;
	};
    
private:
    int m_id;
    std::vector<GraphEdge*> m_edges;
};

class GraphEdge {
public:
    GraphEdge(GraphNode *n1, GraphNode *n2);
    ~GraphEdge();
	GraphNode *& from() { return m_from;}
	GraphNode *& to() { return m_to;}

	void print();
private:
    GraphNode *m_from, *m_to;
};

class DualGraph
{
public:
	DualGraph(void);
	~DualGraph(void);

    //node id start from 1
    GraphNode *getNode(int id) { return m_nodes[id - 1]; }
    GraphEdge *getEdge(int nid0, int nid1); 

    GraphNode *addNode();
    GraphEdge *addEdge(GraphNode *from, GraphNode *to);

    //print
	void print();

    //in and out

    void saveGraph(const char *filename);
	void saveGV(const char *filename);

    void loadGraph(const char *filename);
	void loadMesh(const char *filename);
	void loadTetMesh(const char *filename);

	void saveMetis(const char *filename);

    //iterators
    struct NodeIter {
        NodeIter(DualGraph *graph) : m_it(graph->m_nodes.begin()), m_end(graph->m_nodes.end()) {}
        void operator++() { ++m_it; }
        bool end() { return m_it == m_end; }
        GraphNode* operator*() { return *m_it; }
        std::vector<GraphNode*>::iterator m_it, m_end;
	};

    struct EdgeIter {
        EdgeIter(DualGraph *graph) : m_it(graph->m_edges.begin()), m_end(graph->m_edges.end()) {}
        void operator++() { ++m_it; }
        bool end() { return m_it == m_end; }
        GraphEdge* operator*() { return (*m_it).second; }
        std::map<std::pair<int, int>, GraphEdge*>::iterator m_it, m_end;
	};

private:
	void reset();
    std::vector<GraphNode*> m_nodes;
    std::map<std::pair<int, int>, GraphEdge*> m_edges;

};

