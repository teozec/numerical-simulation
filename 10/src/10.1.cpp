#include <iostream>
#include <fstream>
#include <vector>
#include "TSP.h"
#include "random.h"

// Exercise 10.1
// TSP via Simulated Annealing

using namespace std;

double boltzmann(Individual &individual, double beta)
{
	double l = individual.cost() ;
	return exp(-beta * l);
}

// TSP algorithm
void TSP(Random rnd, const int n_cities, const int n_generations, vector<City> city,
	string s, double pm, double beta, double beta_rate, int n_steps)
{
	// Create a vector containing single individual
	vector<Individual> individuals{Individual{city, rnd}};
	// Create a population containing only our individual
	Population population{individuals, rnd, pm, 0., 0.};

	ofstream out{"data/10.1-" + s + "-out.dat"};
	if (!out.is_open()) {
		cerr << "Could not open data/10.1-" + s + "-out.dat" << endl;
		exit(1);
	}

	// Write the L1 cost of the individual
	out << individuals[0].cost() << endl;

	// At each step we lower the temperature (increase beta)
	for (int i = 0; i < n_steps; i++) {
		// Evolve the individual for n generations at fixed temperature
		for (int j = 0; j < n_generations; j++) {
			Individual son = individuals[0];
			population.mutate(son);	// Mutate the individual
			// Choose the son or the parent using metropolis with Boltzmann probability distribution (using the cost in place of the energy)
			rnd.metropolis<Individual>(individuals[0], son, [beta](Individual &individual) { return boltzmann(individual, beta); });
		}
		out << individuals[0].cost() << endl;
		beta *= beta_rate;	// Decrease temperature
	}

	ofstream best{"data/10.1-" + s + "-best.dat"};
	if (!best.is_open()) {
		cerr << "Could not open data/10.1-" + s + "-best.dat" << endl;
		exit(1);
	}
	// Write best path
	for (int i = 0; i < n_cities; i++)
		best << individuals[0].cities[i].label << endl;
}


int main()
{
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");
	const int n_cities = 32;
	const int n_generations = 3000;

	vector<City> circ;
	ofstream out_cities{"data/10.1-circ-cities.dat"};
	if (!out_cities.is_open()) {
		cerr << "Could not open data/10.1-circ-cities.dat" << endl;
		exit(1);
	}

	// Generate cities placed on a circumference
	for (int i = 0; i < n_cities; i++) {
		double theta = rnd.rannyu(0, 2*M_PI);
		circ.push_back(City{cos(theta), sin(theta), i});
		out_cities << i << '\t' << cos(theta) << '\t' << sin(theta) << endl;
	}
	// Call TSP algorithm
	TSP(rnd, n_cities, n_generations, circ, "circ", 0.4, 0.05, 1.1, 100);
	out_cities.close();

	vector<City> square;
	out_cities.open("data/10.1-square-cities.dat");
	if (!out_cities.is_open()) {
		cerr << "Could not open data/10.1-square-cities.dat" << endl;
		exit(1);
	}

	// Generate cities placed inside a square
	for (int i = 0; i < n_cities; i++) {
		double x = rnd.rannyu();
		double y = rnd.rannyu();
		square.push_back(City{x, y, i});
		out_cities << i << '\t' << x << '\t' << y << endl;
	}
	// Call TSP algorithm
	TSP(rnd, n_cities, n_generations, square, "square", 0.3, 0.05, 1.1, 100);


	return 0;
}
