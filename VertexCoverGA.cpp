#ifndef ORCA_VERTEX_COVER_GA
#define ORCA_VERTEX_COVER_GA

#include "GeneticAlgorithm.cpp"
#include "SimulatedAnnealing.cpp"
#include "HillClimber.cpp"
#include "Graph.cpp"
#include "Chromosome.cpp"

#include <vector>
#include <valarray>

#include <math.h>

using namespace std;

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

		VertexCoverChrom::total_fitness_evaluations++;

		int graph_size = this->graph->size();
		float num_uncovered = this->graph->num_uncovered_by(*this);
		float num_covered = graph_size - num_uncovered;

		float num_selected = 0;
		for (int i = 0; i < this->size(); i++) {
			num_selected += this->at(i);
		}
		float num_unselected = graph_size - num_selected;
		int max_degree = graph->max_degree();

		if (num_selected == 0) return 0;

		float fit;
		//fit = ( num_unselected - 2 * max_degree * num_uncovered );
		if (num_uncovered == 0) {
			//fit = fit * 2;
		}
		fit = ( 2 * num_covered + num_unselected );
		//fit = num_unselected + num_covered;
		//
		//float fit = (num_covered * num_covered * num_covered + num_unselected) * (num_uncovered == 0 ? graph_size : 0.00001);
		this->fitness_cache = fit;
		//this->fitness_cache = ((float) num_covered * (float) num_covered - (float) cover_size) / this->graph->size();

		this->fitness_cache_valid = true;
		return this->fitness_cache;
	}

	virtual void perturb() {
		int n = rand() % this->size();
		at(n) = at(n) ^ 1;
		fitness_cache_valid = false;
	};

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
		VertexCoverChrom child = this->single_point_crossover(other);
		//cout << "P1: " << fitness() << endl;
		//cout << "P2: " << other.fitness() << endl;
		//cout << "C1: " << child.fitness() << endl;
		//cout << endl;
		return child;
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

	virtual VertexCoverChrom two_point_crossover(VertexCoverChrom& other) {
		// single-point crossover
		int size = other.size();
		int N = rand() % size;
		int M = rand() % size;

		VertexCoverChrom child(this->graph);
		child.resize(size);
		
		for (int i = 0; i < size; i++) {
			if (i <= N && i <= M) {
				child[i] = other.at(i);
			} else if (i > N && i <= M) {
				child[i] = this->at(i);
			} else {
				child[i] = other.at(i);
			}
		}
		return child;
	}

	virtual VertexCoverChrom uniform_crossover(VertexCoverChrom& other) {
		int size = other.size();

		VertexCoverChrom child(this->graph);
		child.resize(size);
		
		for (int i = 0; i < size; i++) {
			bool b = rand() % 3;
			if (b) {
				child[i] = this->at(i);
			} else {
				child[i] = other.at(i);
			}
		}
		return child;
	}

	virtual void fixup() {
		int N = size();
		while (! is_viable()) {
			int best_bit = 0;
			int best_uncovered = N;
			
			// randomly
			best_bit = rand() % N;
			/*
			for (int i = 0; i < N; i++) {
				if (at(i) == 0) {
					at(i) = 1;
					if (this->num_uncovered() < best_uncovered) {
						best_bit = i;
						best_uncovered = num_uncovered();
					}
					at(i) = 0;
				}
			}
			*/
			at(best_bit) = 1;
			//cout << bitstring() << endl;
		}
	}

	virtual bool is_viable() {
		return this->graph->num_uncovered_by(*this) == 0;
	}

	int num_uncovered() {
		return this->graph->num_uncovered_by(*this);
	}

	string bitstring() {
		string s = "";
		for (int i = 0; i < this->size(); i++) {
			s.append(this->at(i) ? "1" : "0");
		}
		return s;
	}

	static int total_fitness_evaluations;
private:
	Graph *graph;
	float fitness_cache;
	bool fitness_cache_valid = false;
};

// C++ is so dumb
int VertexCoverChrom::total_fitness_evaluations;

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

	virtual VertexCoverChrom gen_blank() {
		vector<bool> chrom(this->graph.size());
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

	virtual VertexCoverChrom gen_blank() {
		vector<bool> chrom(this->graph.size());
		return VertexCoverChrom(&(this->graph), chrom);
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

class VertexCoverHillClimbing : public HillClimbing<VertexCoverChrom> {
private:
	Graph graph;
public:
	VertexCoverHillClimbing (Graph g) {
		this->graph = g;
	}

	virtual VertexCoverChrom gen_blank() {
		vector<bool> chrom(this->graph.size());
		return VertexCoverChrom(&(this->graph), chrom);
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

#endif
