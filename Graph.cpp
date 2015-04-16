
#ifndef BRAVE_GRAPH
#define BRAVE_GRAPH

#include <iostream>
#include <vector>
#include <set>

using namespace std;
class Graph {
private:
	vector<set<int>> adjacency_list;

public:
	Graph();
	Graph(int vertices);

	int size();

	int add_vertex();
	void add_edge(int v1, int v2);
	int num_uncovered_by(vector<bool> cover);
};

Graph::Graph() {
}

Graph::Graph(int vertices) {
	while (adjacency_list.size() < vertices) {
		add_vertex();
	}
}

int Graph::add_vertex() {
	adjacency_list.push_back(set<int>());
	return adjacency_list.size();
}

void Graph::add_edge(int v1, int v2) {
	adjacency_list[v1].insert(v2);
	adjacency_list[v2].insert(v1);
}

int Graph::num_uncovered_by(vector<bool> cover) {
	vector<bool> covered = cover;
	for (int v = 0; v < adjacency_list.size(); v++) {
		if (cover[v]) {
			for (int w : adjacency_list[v]) {
				covered[w] = true;
			}
			covered[v] = true;
		}
	}

	int num_uncovered = 0;
	for (bool is_covered : covered) {
		num_uncovered += !is_covered;
	}

	return num_uncovered;
}

int Graph::size() {
	return adjacency_list.size();
}

#endif
