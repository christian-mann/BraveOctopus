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

		//C soln = this->gen_blank();
		//soln.fixup();
		//
		C soln = this->gen_random();

		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator (seed);

		while (soln.fitness() < fitness_goal || generations > 0) {
			for (int round = 0; round < rounds; round++) {
				C new_soln = soln;
				for (int i = 0; i < (round % 4) / 3 + 1; i++) {
					new_soln.perturb();
				}
				//new_soln.fixup();


				if (new_soln.fitness() >= soln.fitness()) {
					soln = new_soln;
				} else {
					float diff = new_soln.fitness() - soln.fitness();
					float prob = exp(diff / T);
					float r = uniform_real_distribution<float>(0, 1)(generator);
					if (r < prob && diff != 0) {
						cout << "Accepted worse solution (" << new_soln.fitness() << ") with probability " << prob << endl;
						soln = new_soln;
					}
				}
				/*
				if (new_soln.fitness() > soln.fitness() ||
					r < exp((new_soln.fitness() - soln.fitness()) / T) ) {
					if (new_soln.fitness() != soln.fitness()) {
						//cout << "Accepted new solution with fitness " << new_soln.fitness() << endl;
					}
					soln = new_soln;
				};
				*/

				generations--;
			}

			T = T * this->alpha;
			rounds = rounds * this->beta;

			cout << soln.bitstring() << endl;
			cout << soln << endl;
			cout << generations << endl;
		}

		cout << soln;
	}

	void set_temperature(double temp) {
		this->initial_temperature = temp;
	}

	void set_iterations(double temp) {
		this->initial_rounds = temp;
	}

	void set_goal(float goal) {
		this->fitness_goal = goal;
	}
	

	private:
	float alpha = 0.98; // temperature decay
	float beta = 1.02; // round increase
	double initial_temperature;
	double initial_rounds;

	float fitness_goal = 0;
};

#endif
