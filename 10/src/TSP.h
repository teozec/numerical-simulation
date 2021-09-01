#include <vector>
#include <cmath>
#include <algorithm>
#include "random.h"

// A city consists of x, y coordinates and an integer label
struct City {
	double x, y;
	int label;

	City() {}
	City(double x, double y, int label): x{x}, y{y}, label{label} {}

	// The L1 distance between two cities
	double distance(const City &other) const {
		return std::abs(x - other.x) + std::abs(y - other.y);
	}

	bool operator==(const City &other) {
		return label == other.label;
	}
};

// An individual is an ordered path of cities
class Individual {
public:
	// The cost of an individual is the sum of the distance between cities i and i+1
	double cost() const {
		double c = cities[cities.size()].distance(cities[0]);
		for (size_t i = 0; i < cities.size() - 1; i++)
			c += cities[i].distance(cities[i+1]);
		return c;
	}

	std::vector<City> cities;

	Individual() {}

	// Constructor for Individual
	// It takes a vector of cities as inupt, and stores them in a random order (using rnd), only keeping the first one in the first position.
	Individual(std::vector<City> c, Random &rnd) {
		cities.push_back(c[0]);
		for (size_t i = 1; i < c.size(); i++) {
			for (;;) {
				City city = c[(size_t) rnd.rannyu(1, c.size())];	// Extract random city...
				// .. and push it back if it has not been extracteb yet
				if (std::find(cities.begin() + 1, cities.end(), city) == cities.end()) {
					cities.push_back(city);
					break;
				}
			}
		}
	}

	// operator<, operator>, operator== and operator!= compare the costs of two individuals.
	// In particular, operator< can be used by the C++ std::sort function.
	bool operator<(const Individual &other) const {
		return cost() < other.cost();
	}

	bool operator>(const Individual &other) const {
		return cost() > other.cost();
	}

	bool operator==(const Individual &other) const {
		const double epsilon = 1e-8;
		return std::abs(cost() - other.cost() < epsilon);
	}

	bool operator!=(const Individual &other) const {
		return !(*this == other);
	}
};

// A population contains a vector of individuals that evolve following a genetic algortithm
class Population {
public:
	std::vector<Individual> individuals;

private:
	size_t n_individuals, n_cities;
	Random &rnd;
	double pm;	// Mutation probability
	double pc;	// Crossover probability
	double p;	// Selection parameter

public:
	// Construt a population from a vector of individuals
	Population(std::vector<Individual> individuals, Random &rnd, double pm = 0.1, double pc = 0.5, double p = 3.):
		individuals{individuals}, n_individuals{individuals.size()}, n_cities{individuals[0].cities.size()}, rnd{rnd}, pm{pm}, pc{pc}, p{p} {
		sort();	// The individuals in a population are always sorted from the least costly.
	}

	// Sort the population
	void sort() {
		std::sort(individuals.begin(), individuals.end()); //std::sort uses Individual::operator<.
	}

	// Select an individual randomly, with a greater probability of extracting a less costly one
	Individual select() {
		return individuals[(size_t) n_individuals * std::pow(rnd.rannyu(), p)];
	}

	// Evolve the population
	void new_generation() {
		std::vector<Individual> new_generation_vec{n_individuals};
		for (size_t i = 0; i < n_individuals; i += 2) {
			// Select two parents
			Individual parent1 = select();
			Individual parent2 = select();
			Individual son1, son2;
			// Create son1 and son2 from the parents, possibly with crossover
			crossover(parent1, parent2, son1, son2);
			// Mutate the sons
			mutate(son1);
			mutate(son2);
			// Add the sons to the new generation
			new_generation_vec[i] = son1;
			new_generation_vec[i+1] = son2;
		}
		individuals = new_generation_vec;					 // Replace the old generation
		sort();	 // Sort the population
	}

	// Mutate an individual (each mutation happens with probability pm)
	void mutate(Individual &individual) {
		// Swap two random cities
		if (rnd.rannyu() < pm)
			std::swap(individual.cities[(size_t) rnd.rannyu(1, n_cities)], individual.cities[(size_t) rnd.rannyu(1, n_cities)]);
		// Invert a group of contiguous cities
		if (rnd.rannyu() < pm) {
			int r1 = (int) rnd.rannyu(1, n_cities);
			int r2 = (int) rnd.rannyu(1, n_cities);
			if (r2 > r1)
				std::reverse(individual.cities.begin()+r1, individual.cities.begin()+r2);
			else
				std::reverse(individual.cities.begin()+r2, individual.cities.begin()+r1);
		}
		// Rotate a group of contiguous cities
		if (rnd.rannyu() < pm) {
			std::vector<int> x{(int) rnd.rannyu(1, n_cities), (int) rnd.rannyu(1, n_cities), (int) rnd.rannyu(1, n_cities)};
			std::sort(x.begin(), x.end());
			std::rotate(individual.cities.begin()+x[0], individual.cities.begin()+x[1], individual.cities.begin()+x[2]);
		}
	}

	// Create two sons from the parents, applying crossover with probability pc
	void crossover(Individual parent1, Individual parent2, Individual &son1, Individual &son2) {
		son1 = parent1;
		son2 = parent2;

		// Crossover
		if (rnd.rannyu() < pc) {
			size_t p_cut = (size_t) rnd.rannyu(1, n_cities - 1);	// Random cutting point

			size_t index = p_cut;

			// Place cities after p_cut in son1 depending on the position they appear in parent2
			for (size_t i = 0; i < n_cities; i++) {
				City c = parent2.cities[i];
		  		if (std::find(parent1.cities.begin(), parent1.cities.begin() + p_cut, c) == parent1.cities.begin() + p_cut)
					son1.cities[index++] = c;
			}

			index = p_cut;
			// Place cities after p_cut in son2 depending on the position they appear in parent1
			for (size_t i = 0; i < n_cities; i++) {
				City c = parent1.cities[i];
				if (std::find(parent2.cities.begin(), parent2.cities.begin() + p_cut, c) == parent2.cities.begin() + p_cut)
					son2.cities[index++] = c;
			}
		}
	}
};
