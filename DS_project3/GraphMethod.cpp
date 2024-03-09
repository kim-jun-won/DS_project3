#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <algorithm>

using namespace std;

bool BFS(Graph* graph, char option, int vertex, vector<int>&a)
{
	if (graph == nullptr || vertex <0 || vertex > graph->getSize()) {
		return false;
	}

	bool* visited = new bool[graph->getSize() + 1];
	for (int i = 1; i <= graph->getSize(); i++) {
		visited[i] = false;
	}
	queue<int> q;
	a.clear();

	q.push(vertex);
	visited[vertex] = true;
	
	while (!q.empty())
	{
		int currentVertex = q.front();
		q.pop();
		//
		a.push_back(currentVertex);

		map<int, int> adjacentEdges;
		if (option == 'Y') {
			graph->getAdjacentEdgesDirect(currentVertex, &adjacentEdges);
		}
		else if (option == 'N') {
			graph->getAdjacentEdges(currentVertex, &adjacentEdges);
		}

		for (const auto& edge : adjacentEdges)
		{
			int neighbor = edge.first;
			if (!visited[neighbor]) {
				q.push(neighbor);
				visited[neighbor] = true;
			}
		}
	}

	delete[] visited;
	return true;
	
}

bool DFS(Graph* graph, char option, int vertex, vector<int>&a)
{
	if (graph == nullptr || vertex <0 || vertex > graph->getSize()) {
		return false;
	}

	bool* visited = new bool[graph->getSize() + 1];
	for (int i = 1; i <= graph->getSize(); i++) {
		visited[i] = false;
	}

	stack<int> s;

	s.push(vertex);
	visited[vertex] = true;
	a.push_back(vertex);

	while (!s.empty())
	{
		bool flag = false;
		map<int, int>adjacentEdge;
		int currentVertex = s.top();
		if (option == 'Y')
		{
			graph->getAdjacentEdgesDirect(currentVertex, &adjacentEdge);
		}
		else if (option == 'N')
		{
			graph->getAdjacentEdges(currentVertex, &adjacentEdge);
		}

		for (const auto& edge : adjacentEdge)
		{
			int neighbor = edge.first;
			if (!visited[neighbor])
			{
				visited[neighbor] = true;
				a.push_back(neighbor);
				s.push(neighbor);
				flag = true;
				break;
			}
		}
		if (!flag) s.pop();
	}

	return true;
}

// class Edge
class Edge {
public:
	int node[2];
	int distance;
	Edge(int from, int to, int weight)
	{
		this->node[0] = from;
		this->node[1] = to;
		this->distance = weight;
	}
	bool operator <(Edge& edge)
	{
		return this->distance < edge.distance;
	}
};
// getParent
int getParent(int* set, int x)
{
	if (set[x] == x) return x;
	return set[x] = getParent(set, set[x]);
}

// union parent
void unionParent(int* set, int a, int b)
{
	a = getParent(set, a);
	b = getParent(set, b);

	if (a < b) set[b] = a;
	else set[a] = b;
}

// check if it has same parent node
bool find(int* set, int a, int b)
{
	int a1 = getParent(set, a);
	int b1= getParent(set, b);
	if (a1 == b1) return true;
	else return false;
}

bool Kruskal(Graph* graph, map<int,int> * m)
{
	int m_size = graph->getSize();
	//m = new map<int, int>[ m_size + 1];

	// if graph is nullptr return false
	if (graph == nullptr) return false;
	int size = graph->getSize();
	map<int, int>adjacentEdge;
	
	vector<Edge> v;
	// create Edge
	for (int i = 1; i <= size; i++)
	{
		graph->getAdjacentEdgesDirect(i, &adjacentEdge);
		
		for (const auto& edge : adjacentEdge)
		{
			// i = from , edge.first = to , edge.second = weight
			v.push_back(Edge(i, edge.first, edge.second));
		}
	}
	// 간선 weight 기준 오름차순 정렬
	sort(v.begin(), v.end());

	// parent array
	int* parent = new int[size+1];
	for (int i = 1; i <= size; i++)
	{
		parent[i] = i;
	}

	int sum = 0;
	for (int i = 0; i < v.size(); i++)
	{
		if (!find(parent, v[i].node[0], v[i].node[1]))
		{
			sum += v[i].distance;
			unionParent(parent, v[i].node[0], v[i].node[1]);
			
			m[v[i].node[0]].insert({ v[i].node[1], v[i].distance });
			m[v[i].node[1]].insert({ v[i].node[0], v[i].distance });
		}
	}
	m[0].insert({ sum,0 });

	return true;
}

bool Dijkstra(Graph* graph, char option, int vertex, vector<int> * v, vector<int> & dist)
{
	int size = graph->getSize();
	priority_queue<pair<int, int>> pq;
	
	while (!pq.empty())
		pq.pop();

	int* prev = new int[size + 1];
	for (int i = 1; i <= size; i++) prev[i] = vertex;
	
	dist[vertex] = 0;
	pq.push({ 0,vertex});

	while (!pq.empty())
	{
		map<int, int>adjacentEdge;
		int cur_distance = -pq.top().first;
		int cur_vertex = pq.top().second;
		pq.pop();		
		
		// get AdjacentEdge
		if (option == 'Y'){
			graph->getAdjacentEdgesDirect(cur_vertex, &adjacentEdge);
		}
		else if (option == 'N'){
			graph->getAdjacentEdges(cur_vertex, &adjacentEdge);
		}

		for (auto const edge : adjacentEdge){
			if (edge.second < 0) {
				return false;
			}
			int next_vertex = edge.first;
			int next_dist = cur_distance + edge.second;
		
			if (next_dist < dist[next_vertex]) {
				dist[next_vertex] = next_dist;
				pq.push({ -next_dist, next_vertex });
				v[next_vertex].push_back(cur_vertex);
			}
		}
	}

	int num;
	for (int i = 1; i <= size; i++){
		if (!v[i].empty())
		{
			num = v[i].back();

			while (!v[i].empty())
				v[i].pop_back();
			prev[i] = num;
		}
	}
	
	for (int i = 1; i <= size; i++)
	{
		int p = prev[i];
		v[i].push_back(p);

		while (p != vertex) {
			p = prev[p];
			v[i].push_back(p);
		}
	}

	delete[]prev;

	return true;
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex, vector<int> * v, int * dist)
{
	int size = graph->getSize();
	map<int, int>adjacentEdge;
	int* prev = new int[size + 1];
	

	for (int i = 1; i <= size; i++)
	{
		prev[i] = s_vertex;
	}
	for (int i = 1; i <= size; i++) dist[i] = 99;
	dist[s_vertex] = 0;
	// update first
	for (int j = 1; j <= size; j++)
	{
		for (int i = 1; i <= size; i++)
		{
			if (option == 'Y') {
				graph->getAdjacentEdgesDirect(i, &adjacentEdge);
			}
			else if (option == 'N')
			{
				graph->getAdjacentEdges(i, &adjacentEdge);
			}
			for (const auto edge : adjacentEdge)
			{
				if (dist[i] == 99) continue;
				if (dist[i] + edge.second < dist[edge.first]) {
					dist[edge.first] = dist[i] + edge.second;
					prev[edge.first] = i;
				}
			}
		}
	}
	
	/* update second
	check negative cycle*/
	for (int i = 1; i <= size; i++)
	{
		if (option == 'Y') {
			graph->getAdjacentEdgesDirect(i, &adjacentEdge);
		}
		else if (option == 'N')
		{
			graph->getAdjacentEdges(i, &adjacentEdge);
		}
		for (const auto edge : adjacentEdge)
		{
			if (dist[i] == 99) continue;
			if (i!= s_vertex && (dist[i] + edge.second < dist[edge.first])) {
				// if graph has negative cycle return false
				cout << "negative cycle" << endl;
				return false;
			}
		}
	}
	
	for (int i = 1; i <= size; i++)
	{
		int p = prev[i];
		v[i].push_back(p);

		while (p != s_vertex) {
			p = prev[p];
			v[i].push_back(p);
		}
	}
	
	delete[] prev;
	return true;
}

bool FLOYD(Graph* graph, char option, int ** a)
{
	int size = graph->getSize();
	if (graph == nullptr) return false;

	map<int, int>adjacentEdge;

	for (int i = 1; i <= size; i++) {

		if (option == 'Y')
		{
			graph->getAdjacentEdgesDirect(i, &adjacentEdge);
		}
		else if (option == 'N')
		{
			graph->getAdjacentEdges(i, &adjacentEdge);
		}

		for (const auto& edge : adjacentEdge)
		{
			a[i][edge.first] = edge.second;
		}
	}

	for (int k = 1; k <= size; k++)
	{
		for (int i = 1; i <= size; i++)
		{
			for (int j = 1; j <= size; j++)
			{
				if (a[i][k] + a[k][j] < a[i][j]) {
					if ((i == j) && a[i][j] < 0) return false;
					else a[i][j] = a[i][k] + a[k][j];
				}
			}
		}
	}
	return true;
}

bool KWANGWOON(Graph* graph, int vertex) {
	return true;
}






