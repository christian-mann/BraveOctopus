#include "GeneticAlgorithm.cpp"
#include "Graph.cpp"
#include "Chromosome.cpp"

#include <vector>
#include <valarray>

#include <math.h>

using namespace std;

Graph static_graph;

class VertexCoverChrom : public Chromosome<VertexCoverChrom>, public vector<bool> {

public:
	VertexCoverChrom()
	{
		// this doesn't make sense
		// please don't use it by itself
	}

	~VertexCoverChrom() noexcept
	{ }

	VertexCoverChrom(Graph *g) :
	vector<bool> ()
	{
		this->graph = g;
	}

	VertexCoverChrom(Graph *g, vector<bool> arr) :
	vector<bool>(arr)
	{
		this->graph = g;
	}

	virtual float fitness() {
		if (this->fitness_cache_valid) {
			return this->fitness_cache;
		}

		int cover_size = 0;
		for (int i = 0; i < this->size(); i++) {
			cover_size += this->at(i);
		}

		int num_uncovered = this->graph->num_uncovered_by(*this);
		int size_fitness = this->graph->size() - cover_size - num_uncovered;
		if (num_uncovered > 0) {
			this->fitness_cache = size_fitness / num_uncovered;
		} else {
			this->fitness_cache = size_fitness * 2;
		}

		this->fitness_cache_valid = true;
		return this->fitness_cache;
	}

	virtual VertexCoverChrom mutate(float mutation_rate = 0.05) {
		// randomly flip bits
		VertexCoverChrom child = *this;
		int N = rand() % child.size();
		
		child[N] = child[N] ^ 1;

		return child;
	}

	virtual VertexCoverChrom crossover(VertexCoverChrom& other) {
		return this->uniform_crossover(other);
	}

	virtual VertexCoverChrom single_point_crossover(VertexCoverChrom& other) {
		// single-point crossover
		int size = other.size();
		int N = rand() % size;

		VertexCoverChrom child(this->graph);
		child.resize(size);
		
		for (int i = 0; i < size; i++) {
			if (i <= N) {
				child[i] = other.at(i);
			} else {
				child[i] = this->at(i);
			}
		}
		return child;
	}

	virtual VertexCoverChrom uniform_crossover(VertexCoverChrom& other) {
		// single-point crossover
		int size = other.size();

		VertexCoverChrom child(this->graph);
		child.resize(size);
		
		for (int i = 0; i < size; i++) {
			bool b = rand() % 2;
			if (b) {
				child[i] = this->at(i);
			} else {
				child[i] = other.at(i);
			}
		}
		return child;
	}

	virtual bool is_viable() {
		return this->graph->num_uncovered_by(*this) == 0;
	}

private:
	Graph *graph;
	float fitness_cache;
	bool fitness_cache_valid;
};

class VertexCoverGeneticAlgorithm : public GeneticAlgorithm<VertexCoverChrom> {
private:
	Graph graph;
public:
	VertexCoverGeneticAlgorithm(Graph g) {
		this->graph = g;
	}

	virtual VertexCoverChrom gen_random() {
		vector<bool> chrom(this->graph.size());
		for (int i = 0; i < chrom.size(); i++) {
			chrom[i] = rand() % 2;
		}
		return VertexCoverChrom(&(this->graph), chrom);
	}

	virtual bool should_stop() { return false; }

};
