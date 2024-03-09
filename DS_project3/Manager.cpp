#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool flag;
int count = 0;

vector<string> split(string str, char delimiter)
{
	vector<string> vs;
	string temp;
	for (int i = 0; !(str[i] == 13 || str[i] == '\n' || str[i] == 0); i++)
	{
		if (str[i] == delimiter)
		{
			vs.push_back(temp);
			temp.clear();
			continue;
		}
		temp.push_back(str[i]);
	}
	vs.push_back(temp);
	return vs;
}


Manager::Manager()
{
	graph = nullptr;
	fout.clear();
	fout.open("log.txt", ios::app);
	load = 0;	//Anything is not loaded
}

Manager::~Manager()
{
	if (load)	//if graph is loaded, delete graph
		delete graph;
	if (fout.is_open())	//if fout is opened, close file
		fout.close();	//close log.txt File
}

void Manager::run(const char* command_txt) {
	ifstream fin;	//Command File Input File Stream
	fin.open(command_txt, ios_base::in);//File open with read mode

	if (!fin) { //If command File cannot be read, Print error
		fout << "command file open error" << endl;
		return;	//Return
	}

	string line;
	vector<string> a;

	while (!fin.eof()) {

		getline(fin, line);
		a = split(line, ' ');

		if (a.size() == 0) {
			cout << "Read Error" << endl;
		}
		// LOAD
		else if (a.size() == 2 && a[0] == "LOAD")
		{
			// LOAD file
			if ( !LOAD(a[1].c_str()) )
			{
				printErrorCode(100);
			}
		}
		else if (a.size() == 1 && a[0] == "PRINT")
		{
			if (!PRINT())
			{
				printErrorCode(200);
			}
		}
		else if (a.size() == 3 && a[0] == "BFS")
		{
			if (a[1] != "Y" && a[1] != "N") printErrorCode(1000);
			else {
				const char* m_option = a[1].c_str();
				int m_size = stoi(a[2]);
				mBFS(*m_option, m_size);
			}
			
		}
		else if (a.size() == 3 && a[0] == "DFS")
		{
			if (a[1] != "Y" && a[1] != "N") printErrorCode(1000);
			else {
				const char* m_option = a[1].c_str();
				int m_size = stoi(a[2]);
				mDFS(*m_option, m_size);
			}
		}
		else if (a.size() == 1 && a[0] == "KRUSKAL")
		{
			if (!mKRUSKAL())
			{
				printErrorCode(600);
			}
		}
		// DIJKSTRA
		else if (a.size() == 3 && a[0] == "DIJKSTRA")
		{
			char op;
			if (a[1] != "Y" && a[1] != "N") {
				printErrorCode(1000);
			}
			else {
				if (a[1] == "Y") op = 'Y';
				else op = 'N';

				if (!mDIJKSTRA(op, stoi(a[2])) )
				{
					printErrorCode(700);
				}
			}
		}
		else if (a.size() == 2 && a[0] == "FLOYD")
		{
			char op;
			if (a[1] != "Y" && a[1] != "N") {
				printErrorCode(1000);
			}
			else {
				if (a[1] == "Y") op = 'Y';
				else op = 'N';

				if ( !mFLOYD(op) )
				{
					printErrorCode(900);
				}
			}
		}
		else if (a.size() == 4 && a[0] == "BELLMANFORD")
		{
			char op;
			if (a[1] != "Y" && a[1] != "N") {
				printErrorCode(1000);
			}
			else {
				if (a[1] == "Y") op = 'Y';
				else op = 'N';
				int start = stoi(a[2]);
				int end = stoi(a[3]);

				if (!mBELLMANFORD(op, start, end)) {
					printErrorCode(800);
				}
			}
		}
		else {
				printErrorCode(1000);
		}

		a.clear();
	}
	fin.close();
	fout.clear();
	return;
}

bool Manager::LOAD(const char* filename)
{
	ifstream a;
	a.open(filename);

	if (!a.is_open())
	{
		return false;
	}

	string line;
	vector<string> data;

	///////////////////////////////////////////////////
	getline(a, line);
	data = split(line, ' ');
	if (data.size() == 1 && data[0] == "L") {
		flag = false;
	}
	else if (data.size() == 1 && data[0] == "M") {
		flag = true;
	}
	data.clear();


	/////////////////////////////////////////////////////
	getline(a, line);
	data = split(line, ' ');
	// graph size
	int size = stoi(data[0]);
	data.clear();

	// create List_graph and insert
	if (!flag)
	{
		graph = new ListGraph(flag, size);
		int from = 0; int to; int weight;
		while (!a.eof())
		{
			//cin.ignore();
			getline(a, line);
			data = split(line, ' ');
			if (data[0] == "") break;
			if (data.size() == 1) from = stoi(data[0]);
			else if (data.size() == 2) {
				to = stoi(data[0]); weight = stoi(data[1]);
				graph->insertEdge(from, to, weight);
			}
			data.clear();
		}
	}
	// create Matrix_graph and insert
	else {
		graph = new MatrixGraph(flag, size);
		int from = 1; int to; int weight;
		while (!a.eof())
		{
			//cin.ignore();
			getline(a, line);
			data = split(line, ' ');
			if (data[0] == "") break;
			
			if (data.size() == size)
			{
				for (int i = 0; i < size; i++)
				{
					graph->insertEdge(from, i + 1, stoi(data[i]));
				}
			}
			from++;
			data.clear();
		}
	}
	
	a.close();

	fout << "=================LOAD==============" << endl;
	fout << "Success" << endl;
	fout << "===================================" << endl;

	return true;
}

bool Manager::PRINT()
{
	if (graph->printGraph(&fout)) return true;
	else return false;
	
}

bool Manager::mBFS(char option, int vertex)
{
	vector<int> p_BFS;
	if (BFS(this->graph, option, vertex, p_BFS))
	{
		fout << "==========BFS============" << endl;
		if (option == 'Y') fout << "Directed";
		else if (option == 'N') fout << "Undirected";
		fout << " Graph BFS result" << endl;
		fout << "startvertex: " << vertex << endl;
		for (int i = 0; i < p_BFS.size(); i++)
		{
			if (i == p_BFS.size() - 1) {
				fout << p_BFS[i] << endl;
			}
			else {
				fout << p_BFS[i] << "->";
			}
		}
		fout << "==========================" << endl;
		return true;
	}
	else {
		return false;
	}
}

bool Manager::mDFS(char option, int vertex)
{
	vector<int> p_DFS;
	if (DFS(this->graph, option, vertex, p_DFS))
	{
		fout << "============DFS===============" << endl;
		if (option == 'Y') fout << "Directed";
		else if (option == 'N') fout << "Undirected";
		fout << " Graph DFS result" << endl;
		fout << "startvertex: " << vertex << endl;
		for (int i = 0; i < p_DFS.size(); i++)
		{
			if (i == p_DFS.size() - 1) {
				fout << p_DFS[i] << endl;
			}
			else {
				fout << p_DFS[i] << "->";
			}
		}
		fout << "==========================" << endl;
		return true;
	}
	else {
		return false;
	}

}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	int size = this->graph->getSize();
	vector<int>* v;
	vector<int> dist(size+1,999);
	v = new vector<int>[size + 1];

	if (Dijkstra(graph, option, vertex, v , dist))
	{
		fout << "============Dijkstra===============" << endl;
		if (option == 'Y') fout << "Directed";
		else if (option == 'N') fout << "Undirected";
		fout << "Graph Dijkstra result" << endl;
		fout << "startvertex: " << vertex << endl;
		for (int i = 1; i <= size; i++)
		{
			if (i != vertex)
			{
				fout << "[" << i << "] ";

				if (dist[i] == 999)
				{
					fout << "x" << endl;
				}
				else if (v[i].size() != 0)
				{
					for (int j = v[i].size() - 1; j >= 0; j--)
					{
						fout << v[i][j] << " ->";
					}
					fout << i << "(" << dist[i] << ")" << endl;
				}
				

			}
		}
		fout << "==================================" << endl;

	}
	else {
		return false;
	}
	return true;
}

bool Manager::mKRUSKAL()
{
	int size = this->graph->getSize();
	map<int, int>* m = new map<int, int>[size + 1];

	if (Kruskal(graph, m))
	{
		fout << "========Kruskal======" << endl;
		for (int i = 1; i <= size; i++)
		{
			fout << "[" << i << "]" << "	";
			for (const auto itr : m[i])
			{
				fout << itr.first << "(" << itr.second << ")";
			}
			fout << endl;
		}
		fout << "cost : " <<  m[0].begin()->first << endl;
		return true;
	}
	else return false;
	
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	int size = graph->getSize();
	int* dist = new int[size + 1];
	vector<int>* v = new vector<int>[size + 1];
	if (Bellmanford(graph, option, s_vertex, e_vertex, v, dist))
	{
		fout << "===========Bellman-Ford===========" << endl;
		if (option == 'Y') fout << "Directed";
		else if (option == 'N') fout << "Undirected";
		fout << "Graph Bellman-For result" << endl;	
		if (v[e_vertex].size() != 0) {
			for (int j = v[e_vertex].size() - 1; j >= 0; j--)
			{
				fout << v[e_vertex][j] << "->";
			}
			fout << e_vertex << endl;
			fout << "cost:" << dist[e_vertex] << endl;
			fout << "===================================" << endl << endl;
		}
		return true;
	}
	else {
		fout << "negative cycle" << endl;
		return false;
	}

}

bool Manager::mFLOYD(char option)
{
	int size = graph->getSize();
	// create new 2D array a
	int** a = new int*[size + 1];
	for (int i = 0; i < size + 1; i++)
	{
		a[i] = new int[size + 1];
	}

	for (int i = 0; i <= size; i++)
	{
		for (int j = 0; j <= size; j++)
		{
			if (i == j) a[i][j] = 0;
			a[i][j] = 99;
		}
	}

	if (FLOYD(graph, option, a)) {
		fout << "==========FLOYD==========" << endl;
		fout << "     ";
		for (int i = 1; i <= size; i++) fout << "[" << i << "] ";
		fout << endl;
		for (int i = 1; i <= size; i++)
		{
			fout << "[" << i << "]" << " ";
			for (int j = 1; j <= size; j++)
			{
				fout << a[i][j] << "   ";
			}
			fout << endl;
		}
		fout << "==========================" << endl << endl;
		return true;
	}
	else {
		return false;
	}
	
}

bool Manager::mKwoonWoon(int vertex)
{
	return true;
}

void Manager::printErrorCode(int n)
{
	fout << "========ERROR=======" << endl;
	fout << n << endl;
	fout << "====================" << endl << endl;
}


