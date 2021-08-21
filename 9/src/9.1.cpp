#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <numeric>
#include "TSP.h"

using namespace std;

double cost_accumulate(vector<Individual> individuals, int n_individuals)
{
	return accumulate(individuals.begin(), individuals.begin() + n_individuals, individuals[0].cost(),
		[](const double x, const Individual &y) { return x + y.cost(); }) / n_individuals;
}

void TSP(Random rnd, const int n_cities, const int n_individuals, const int n_generations,
	vector<City> city, string s, double pm, double pc, double p)
{
	vector<Individual> individuals;
	for (int i = 0; i < n_individuals; i++)
		individuals.push_back(Individual{city, rnd});

	Population population{individuals, rnd, pm, pc, p};
	ofstream out{"data/9.1-" + s + "-out.dat"};
	if (!out.is_open()) {
		cerr << "Could not open data/9.1-" + s + "-out.dat" << endl;
		exit(1);
	}

	out << population.individuals[0].cost() << '\t' << cost_accumulate(population.individuals, n_individuals / 2) << endl;
	for (int i = 0; i < n_generations; i++) {
		population.new_generation();
		out << population.individuals[0].cost() << '\t' << cost_accumulate(population.individuals, n_individuals / 2) << endl;
	}

	ofstream best{"data/9.1-" + s + "-best.dat"};
	if (!best.is_open()) {
		cerr << "Could not open data/9.1-" + s + "-best.dat" << endl;
		exit(1);
	}
	for (int i = 0; i < n_cities; i++)
		best << population.individuals[0].cities[i].label << endl;
}

int main()
{
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");
	const int n_cities = 32;
	const int n_individuals = 5000;
	const int n_generations = 300;

	vector<City> circ;
	ofstream out_cities{"data/9.1-circ-cities.dat"};
	if (!out_cities.is_open()) {
		cerr << "Could not open data/9.1-circ-cities.dat" << endl;
		exit(1);
	}
	for (int i = 0; i < n_cities; i++) {
		double theta = rnd.rannyu(0, 2*M_PI);
		circ.push_back(City{cos(theta), sin(theta), i});
		out_cities << i << '\t' << cos(theta) << '\t' << sin(theta) << endl;
	}
	TSP(rnd, n_cities, n_individuals, n_generations, circ, "circ", 0.15, 0.7, 2.);

	vector<City> square;
	out_cities.close();
	out_cities.open("data/9.1-square-cities.dat");
	if (!out_cities.is_open()) {
		cerr << "Could not open data/9.1-square-cities.dat" << endl;
		exit(1);
	}
	for (int i = 0; i < n_cities; i++) {
		double x = rnd.rannyu();
		double y = rnd.rannyu();
		square.push_back(City{x, y, i});
		out_cities << i << '\t' << x << '\t' << y << endl;
	}
	TSP(rnd, n_cities, n_individuals, n_generations, square, "square", 0.1, 0.7, 2.);

	return 0;
}
