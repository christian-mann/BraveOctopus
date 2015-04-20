#ifndef ORCA_SIMULATED_ANNEALING
#define ORCA_SIMULATED_ANNEALING

#include "GeneticAlgorithm.cpp"

template <class C>
class SimulatedAnnealing : public GeneticAlgorithm<C> {
	public:

	void initialize(void) {
	}

	C run(int generations) {
		double T = this->initial_temperature;
		double rounds = this->initial_rounds;

		//C soln = this->gen_blank();
		//soln.fixup();
		//
		C soln = this->gen_random();
		C best_soln = soln;

		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator (seed);

		while (best_soln.fitness() < this->fitness_goal || generations > 0) {
			int prev_best_fit = best_soln.fitness();
			for (int round = 0; round < rounds; round++) {
				C new_soln = soln;
				new_soln.perturb();


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

				if (soln.fitness() > best_soln.fitness()) {
					best_soln = soln;
				}

				generations--;
			}

			if (prev_best_fit == best_soln.fitness() && T <= 0.2) {
				// kick
				cout << "KICK" << endl;
				T *= 2;
			}

			T = T * this->alpha;
			rounds = rounds * this->beta;

			cout << soln.bitstring() << endl;
			cout << soln << endl;
			cout << generations << endl;
		}

		cout << best_soln;
		return best_soln;
	}

	void set_temperature(double temp) {
		this->initial_temperature = temp;
	}

	void set_iterations(double temp) {
		this->initial_rounds = temp;
	}


	private:
	float alpha = 0.95; // temperature decay
	float beta = 1.02; // round increase
	double initial_temperature;
	double initial_rounds;
};

#endif
