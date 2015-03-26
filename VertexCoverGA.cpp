#include "GeneticAlgorithm.cpp"
#include "Graph.cpp"

#include <vector>
#include <valarray>

#include <math.h>

using namespace std;

class VertexCoverGeneticAlgorithm : public GeneticAlgorithm<vector<bool>> {
private:
	Graph graph;
public:
	VertexCoverGeneticAlgorithm(Graph g) {
		this->graph = g;
	}

	virtual tuple<vector<bool>, vector<bool>> crossover(vector<bool>& parent1, vector<bool>& parent2) {
		return uniform_crossover(parent1, parent2);
	}

	virtual tuple<vector<bool>, vector<bool>> single_point_crossover(vector<bool>& parent1, vector<bool>& parent2) {
		// single-point crossover
		int size = parent1.size();
		int N = rand() % size;

		vector<bool> child1(size),
			         child2(size);
		
		for (int i = 0; i < parent1.size(); i++) {
			if (i <= N) {
				child1[i] = parent1[i];
				child1[i] = parent2[i];
			} else {
				child1[i] = parent2[i];
				child2[i] = parent1[i];
			}
		}

		return tuple<vector<bool>, vector<bool>>(child1, child2);
	}

	virtual tuple<vector<bool>, vector<bool>> uniform_crossover(vector<bool>& parent1, vector<bool>& parent2) {
		// uniform crossover
		int size = parent1.size();

		vector<bool> child1(size),
			         child2(size);

		for (int i = 0; i < size; i++) {
			bool b = rand() % 2;
			if (b) {
				child1[i] = parent1[i];
				child2[i] = parent2[i];
			} else {
				child1[i] = parent2[i];
				child2[i] = parent1[i];
			}
		}

		return tuple<vector<bool>, vector<bool>>(child1, child2);
	}

	virtual vector<bool> mutate(vector<bool>& parent) {
		// randomly flip bits
		vector<bool> child = parent;
		int N = rand() % child.size();

		// apparently this is invalid??
		//child[N] ^= 1;
		
		child[N] = child[N] ^ 1;

		return child;
	}

	virtual float fitness_function(vector<bool>& chrom) {
		static int fn_count = 0;
		fn_count++;
		if (fn_count % 100 == 0)
			cout << "fn_count = " << fn_count << endl;

		int cover_size = 0;
		for (int i = 0; i < chrom.size(); i++) {
			cover_size += chrom[i];
		}

		int size_fitness = this->graph.size() - cover_size - this->graph.num_uncovered_by(chrom);
		if (this->graph.num_uncovered_by(chrom) > 0) {
			return size_fitness / this->graph.num_uncovered_by(chrom);
		} else {
			return size_fitness * 2;
		}
	}

	virtual bool is_viable(vector<bool>& chrom) {
		return this->graph.num_uncovered_by(chrom) == 0;
	}

	virtual vector<bool> gen_random() {
		vector<bool> chrom(this->graph.size());
		for (int i = 0; i < chrom.size(); i++) {
			chrom[i] = rand() % 2;
		}
		return chrom;
	}

	virtual bool should_stop() { return false; }

};
