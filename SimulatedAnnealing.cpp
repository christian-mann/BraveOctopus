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

		for (int i = 0; i < generations; i++) {
			cout << "round " << i << endl;
			for (int round = 0; round < rounds; round++) {
				C new_soln = soln.mutate();

				float r = uniform_real_distribution<float>(0, 1)(generator);

				if (new_soln.fitness() > soln.fitness() ||
					r < exp((new_soln.fitness() - soln.fitness()) / T) ) {
					soln = new_soln;
					cout << "Accepted new solution with fitness " << soln.fitness() << endl;
				};
			}

			T = T * this->alpha;
			rounds = rounds * this->beta;

			for (int k = 0; k < soln.size(); k++) {
				cout << soln[k];
			}
			cout << endl;

		}
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
