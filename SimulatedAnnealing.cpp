#ifndef ORCA_SIMULATED_ANNEALING
#define ORCA_SIMULATED_ANNEALING

#include "GeneticAlgorithm.cpp"

template <class C>
class SimulatedAnnealing : public GeneticAlgorithm<C> {
	public:
	
	void initialize(void) {
	}

	void run(int generations) {
		double T = this->initial_temperature;
		double rounds = this->initial_rounds;

		C soln = this->gen_random();

		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator (seed);

		while (generations > 0) {
			for (int round = 0; round < rounds; round++) {
				C new_soln = soln.mutate(this->mutation_rate);

				float r = uniform_real_distribution<float>(0, 1)(generator);

				if (new_soln.fitness() > soln.fitness() ||
					r < exp((new_soln.fitness() - soln.fitness()) / T) ) {
					if (new_soln.fitness() != soln.fitness()) {
						cout << "Accepted new solution with fitness " << new_soln.fitness() << endl;
					}
					soln = new_soln;
				};

				generations--;
			}

			T = T * this->alpha;
			rounds = rounds * this->beta;

			cout << soln.bitstring() << endl;
		}

		cout << soln;
	}

	void set_temperature(double temp) {
		this->initial_temperature = temp;
	}

	void set_iterations(double temp) {
		this->initial_rounds = temp;
	}

	private:
	float alpha = 0.95;
	float beta = 1.05;
	double initial_temperature;
	double initial_rounds;
};

#endif
