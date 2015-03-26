#ifndef BRAVE_CHROMOSOME
#define BRAVE_CHROMOSOME

#include <type_traits>

template <class T> 
class Chromosome {
	public:
		Chromosome() {
			//static_assert(is_base_of<Chromosome, T>::value, "T must be derived from Chromosome");
		}

		virtual float fitness() = 0;
		virtual T mutate(float mutation_rate = 0.05) = 0;
		virtual T crossover(T& other) = 0;
		virtual bool is_viable() = 0;
};

#endif
