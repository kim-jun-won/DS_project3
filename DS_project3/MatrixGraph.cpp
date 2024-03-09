#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	if (type)
	{
		m_Mat = new int* [size + 1];
		for (int i = 0; i < size + 1; i++)
		{
			m_Mat[i] = new int[size + 1];
		}
	}

	for (int i = 1; i <= size; i++)
	{
		for (int j = 1; j <= size; j++)
		{
			m_Mat[i][j] = 0;
		}
	}
	
}

MatrixGraph::~MatrixGraph()
{

}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	if (vertex <1 || vertex > this->getSize() || m == nullptr)
		return;
	// initialize map
	m->clear();

	for (int i = 1; i <= this->getSize(); i++)
	{
		if (m_Mat[vertex][i] != 0) {
			m->insert({ i, m_Mat[vertex][i] });
		}
	}

	for (int i = 1; i <= this->getSize(); i++)
	{
		if (m_Mat[i][vertex] != 0) {
			m->insert({ i, m_Mat[i][vertex] });
		}
	}
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{

	if (vertex <1 || vertex > this->getSize() || m == nullptr)
		return;
	// initialize map
	m->clear();

	for (int i = 1; i <= this->getSize(); i++)
	{
		if (m_Mat[vertex][i] != 0) {
			m->insert({ i, m_Mat[vertex][i] });
		}
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)
{
	m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(ofstream* fout)
{

	int size = this->m_Size;
	*fout << "===========PRINT===============" << endl;
	*fout << "    ";
	for (int i = 1; i <= size; i++) *fout << "[" << i << "]" << " ";
	*fout << endl;

	for (int i = 1; i <= size; i++)
	{
		*fout << "[" << i << "]" << " ";
		for (int j = 1; j <= size; j++)
		{
			*fout << m_Mat[i][j] << "   ";
			if (j / 2 == 0) *fout << " ";
		}
		*fout << endl;
	}
	return true;
}
