#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	// create map => size만큼
	if (!type) {
		m_List = new map<int,int>[size+1];
	}
}

ListGraph::~ListGraph()
{

}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)	 //Definition of getAdjacentEdges(No Direction == Undirected)
{
	if (vertex <1 || vertex > this->getSize() || m == nullptr)
		return;
	// initialize map
	m->clear();

	// 해당 정점이 가리키고 있는 edge 들 추가
	for (auto& edge : m_List[vertex]) {
		m->insert(edge);
	}

	for (int i = 1; i <= this->getSize(); i++)
	{
		for (auto& edge : m_List[i])
		{
			if(edge.first == vertex )
				m->insert({ i, edge.second });
		}
	}
}

void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)	//Definition of getAdjacentEdges(Directed graph)
{
	if (vertex <1 || vertex > this->getSize() || m == nullptr)
		return;
	// initialize map
	m->clear();

	for (auto& edge : m_List[vertex]) {
		m->insert(edge);
	}
}

void ListGraph::insertEdge(int from, int to, int weight) //Definition of insertEdge
{
	// insert in map map[from] { [to] , [weight] } 
	m_List[from].insert({ to, weight });
}

bool ListGraph::printGraph(ofstream* fout)	//Definition of print Graph
{
	if (m_List == nullptr)
	{
		return false;
	}

	int size = this->getSize();
	*fout << "================PRINT================" << endl;
	for (int i = 1; i <= size; i++)
	{
		*fout << "[" << i << "]" << " ->";
		
		for (auto iter = m_List[i].begin(); iter != m_List[i].end(); iter++)
		{
			if (iter == --m_List[i].end() ) {
				*fout << "(" << iter->first << "," << iter->second << ")" << endl;
			}
			else {
				*fout << "(" << iter->first << "," << iter->second << ")" << " -> ";
			}
		}
	}
	*fout << "=====================================" << endl;

	return true;

}