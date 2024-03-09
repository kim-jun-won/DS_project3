#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"

bool BFS(Graph* graph, char option, int vertex, vector<int>&a);
bool DFS(Graph* graph, char option, int vertex, vector<int>&a);
bool KWANGWOON(Graph* graph, int vertex);
bool Kruskal(Graph* graph, map<int, int>* m);
bool Dijkstra(Graph* graph, char option, int vertex, vector<int> *v, vector<int> & dist);    //Dijkstra
bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex, vector<int> * v, int * dist); //Bellman - Ford
bool FLOYD(Graph* graph, char option, int ** a);   //FLoyd

#endif
