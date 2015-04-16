#ifndef ORCA_GREEDY_SOLVER
#define ORCA_GREEDY_SOLVER

#include "Graph.cpp"
#include "VertexCoverGA.cpp"

class GreedySolver {

public:
	GreedySolver(Graph g) {
		this->graph = g;
	}

	VertexCoverChrom solve() {
		// start with chrom of all zeros
		// find bit to flip to change those to all ones
		// continue
		cout << "solve()" << endl;
		VertexCoverChrom chrom(&(this->graph), vector<bool>(this->graph.size()));
		int N = graph.size();

		while (! chrom.is_viable()) {
			int best_bit = 0;
			int best_uncovered = N;
			for (int i = 0; i < N; i++) {
				if (chrom[i] == 0) {
					chrom[i] = 1;
					if (chrom.num_uncovered() <= best_uncovered) {
						best_bit = i;
						best_uncovered = chrom.num_uncovered();
					}
					chrom[i] = 0;
				}
			}

			chrom[best_bit] = 1;
			cout << chrom.bitstring() << endl;
		}

		return chrom;
	}

private:
	Graph graph;
};

#endif
