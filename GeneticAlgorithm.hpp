#ifndef BRAVE_GENETICALGORITHM_H
#define BRAVE_GENETICALGORITHM_H
#include <tuple>
#include <vector>

template <class C>
class GeneticAlgorithm {
    public:
    // must override me
    virtual std::tuple<C&, C&> crossover(C& chrom1, C& chrom2) = 0;
    virtual C gen_random(void) = 0;
    virtual float fitness(C& chrom) = 0;

    // may override me
    virtual C& mutate(C& chrom) = 0;
    virtual bool should_stop(GeneticAlgorithm &ga) = 0;

    // call me
    void set_mutation_rate(float r);
    void set_crossover_rate(float r);
    void set_population_size(float r);

    void initialize(void);
    void run(int generations);

    private:
	std::vector<C&> population;

	float mutation_rate,
		  crossover_rate;
	int population_size;
};

#endif
