#ifndef ORCA_HILL_CLIMBING
#define ORCA_HILL_CLIMBING

#include "GeneticAlgorithm.cpp"

template <class C>
class HillClimbing : public GeneticAlgorithm<C> {
	public:
	
	void initialize(void) {
	}

	void run(int generations) {

		C best;

		for (int i = 0; i < 10; i++) {
			C soln = this->gen_random();
			int local_generations = generations / 10;
			while (local_generations > 0) {
				C new_soln = soln.mutate(this->mutation_rate);

				if (new_soln.fitness() > soln.fitness()) {
					cout << "Accepted new solution with fitness " << new_soln.fitness() << endl;
					soln = new_soln;
				};

				local_generations--;

				if (local_generations % 10000 == 0)
					cout << soln.bitstring() << endl;
			}
			cout << soln;
			
			if (i == 0 || soln.fitness() > best.fitness()) {
				best = soln;
			}
		}

		cout << "best " << best;
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

