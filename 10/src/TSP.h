#include <vector>
#include <cmath>
#include <algorithm>
#include "random.h"

struct City {
	double x, y;
	int label;

	City() {}
	City(double x, double y, int label): x{x}, y{y}, label{label} {}

	double distance(const City &other) const {
		return std::abs(x - other.x) + std::abs(y - other.y);
	}

	bool operator==(const City &other) {
		return label == other.label;
	}
};

class Individual {
public:
	double cost() const {
		double c = cities[cities.size()].distance(cities[0]);
		for (size_t i = 0; i < cities.size() - 1; i++)
			c += cities[i].distance(cities[i+1]);
		return c;
	}

	std::vector<City> cities;

	Individual() {}
	Individual(std::vector<City> c, Random &rnd) {
		cities.push_back(c[0]);
		for (size_t i = 1; i < c.size(); i++) {
			for (;;) {
				City city = c[(size_t) rnd.rannyu(1, c.size())];
				if (std::find(cities.begin() + 1, cities.end(), city) == cities.end()) {
					cities.push_back(city);
					break;
				}
			}
		}
	}

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

class Population {
public:
	std::vector<Individual> individuals;

private:
	size_t n_individuals, n_cities;
	Random &rnd;
	double pm, pc, p;

public:
	Population(std::vector<Individual> individuals, Random &rnd, double pm = 0.1, double pc = 0.5, double p = 3.):
		individuals{individuals}, n_individuals{individuals.size()}, n_cities{individuals[0].cities.size()}, rnd{rnd}, pm{pm}, pc{pc}, p{p} {
		sort();
	}

	void sort() {
		std::sort(individuals.begin(), individuals.end());
	}

	Individual select() {
		return individuals[(size_t) n_individuals * std::pow(rnd.rannyu(), p)];
	}

	void new_generation() {
		std::vector<Individual> new_generation_vec{n_individuals};
		for (size_t i = 0; i < n_individuals; i += 2) {
			Individual parent1 = select();
			Individual parent2 = select();
			Individual son1, son2;
			crossover(parent1, parent2, son1, son2);
			mutate(son1);
			mutate(son2);
			new_generation_vec[i] = son1;
			new_generation_vec[i+1] = son2;
		}
		individuals = new_generation_vec;
		sort();
	}

	void mutate(Individual &individual) {
		if (rnd.rannyu() < pm)
			std::swap(individual.cities[(size_t) rnd.rannyu(1, n_cities)], individual.cities[(size_t) rnd.rannyu(1, n_cities)]);
		if (rnd.rannyu() < pm) {
			int r1 = (int) rnd.rannyu(1, n_cities);
			int r2 = (int) rnd.rannyu(1, n_cities);
			if (r2 > r1)
				std::reverse(individual.cities.begin()+r1, individual.cities.begin()+r2);
			else
				std::reverse(individual.cities.begin()+r2, individual.cities.begin()+r1);
		}
		if (rnd.rannyu() < pm) {
			std::vector<int> x{(int) rnd.rannyu(1, n_cities), (int) rnd.rannyu(1, n_cities), (int) rnd.rannyu(1, n_cities)};
			std::sort(x.begin(), x.end());
			std::rotate(individual.cities.begin()+x[0], individual.cities.begin()+x[1], individual.cities.begin()+x[2]);
		}
	}

	void crossover(Individual parent1, Individual parent2, Individual &son1, Individual &son2) {
		son1 = parent1;
		son2 = parent2;

		if (rnd.rannyu() < pc) {
			size_t p_cut = (size_t) rnd.rannyu(1, n_cities - 1);

			size_t index = p_cut;

			for (size_t i = 0; i < n_cities; i++) {
				City c = parent2.cities[i];
		  		if (std::find(parent1.cities.begin(), parent1.cities.begin() + p_cut, c) == parent1.cities.begin() + p_cut)
					son1.cities[index++] = c;
			}

			index = p_cut;
			for (size_t i = 0; i < n_cities; i++) {
				City c = parent1.cities[i];
				if (std::find(parent2.cities.begin(), parent2.cities.begin() + p_cut, c) == parent2.cities.begin() + p_cut)
					son2.cities[index++] = c;
			}
		}
	}
};
