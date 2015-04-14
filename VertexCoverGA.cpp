#include "GeneticAlgorithm.cpp"
#include "SimulatedAnnealing.cpp"
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

		if (cover_size == 0) return 0;

		int num_covered = this->graph->size() - this->graph->num_uncovered_by(*this);
		this->fitness_cache = (float) num_covered * (float) num_covered / (float) cover_size;

		this->fitness_cache_valid = true;
		return this->fitness_cache;
	}

	virtual VertexCoverChrom mutate(float mutation_rate = 0.05) {
		// randomly flip bits
		VertexCoverChrom child = *this;
		child.fitness_cache_valid = false;
		
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator (seed);

		for (int i = 0; i < child.size(); i++) {
			float r = uniform_real_distribution<float>(0, 1)(generator);
			if (r < mutation_rate) {
				child[i] = child[i] ^ 1;
			}
		}

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

	int num_uncovered() {
		return this->graph->num_uncovered_by(*this);
	}

private:
	Graph *graph;
	float fitness_cache;
	bool fitness_cache_valid = false;
};

ostream &operator<<(ostream &os, VertexCoverChrom &m) {
	os << "vertex count: " << accumulate(m.begin(), m.end(), 0) << endl;
	os << "vertex set: ";
	for (int i = 0; i < m.size(); i++) {
		if (m[i]) {
			os << i << " ";
		}
	}
	os << endl;
	os << "num uncovered: " << m.num_uncovered() << endl;
	os << "fitness: " << m.fitness() << endl;
	return os;
}

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

class VertexCoverSimulatedAnnealing : public SimulatedAnnealing<VertexCoverChrom> {
private:
	Graph graph;
public:
	VertexCoverSimulatedAnnealing(Graph g) {
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
