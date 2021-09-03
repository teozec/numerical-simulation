/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 1.2
// Test the Central Limit Theorem for uniform and exponential distributions.
// Show that it does not apply to Cauchy distribution.

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

int main()
{
	const int N_throws = 10000;
	const int N_sums[4] = {1, 2, 10, 100};
	const char end_c[4] = {'\t', '\t', '\t', '\n'};

	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of throws into info file
	ofstream out("data/1.2-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.2-info.dat\n";
		return 1;
	}
	out << N_throws << endl;
	for (int i = 0; i < 4; i++)
		out << N_sums[i] << end_c[i];
	out.close();

	// Open data file for uniform distribution
	out.open("data/1.2-unif.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.2-unif.dat\n";
		return 1;
	}

	for (int i = 0; i < N_throws; i++) {
		for (int j = 0; j < 4; j++) {
			int s = 0;
			// Sum N_sums[j] uniformly distributed integer numbers
			for (int z = 0; z < N_sums[j]; z++)
				s += (int) rnd.rannyu(1., 7.);
			out << (double) s / N_sums[j] << end_c[j];
		}
	}
	out.close();

	out.open("data/1.2-exp.dat");
	if (!out.is_open()) {
		cerr << "Could not open 1.2-exp.dat\n";
		return 1;
	}
	for (int i = 0; i < N_throws; i++) {
		for (int j = 0; j < 4; j++) {
			double s = 0.;
			// Sum N_sums[j] exponentially distributed numbers
			for (int z = 0; z < N_sums[j]; z++)
				s += rnd.exp(1.);
			out << s/N_sums[j] << end_c[j];
		}
	}
	out.close();

	out.open("data/1.2-cauchy.dat");
	if (!out.is_open()) {
		cerr << "Could not open 1.2-cauchy.dat\n";
		return 1;
	}
	for (int i = 0; i < N_throws; i++) {
		for (int j = 0; j < 4; j++) {
			double s = 0.;
			// Sum N_sums[j] Cauchy distributed numbers
			for (int z = 0; z < N_sums[j]; z++)
				s += rnd.cauchy(0., 1.);
			out << s/N_sums[j] << end_c[j];
		}
	}
	out.close();

	rnd.save_seed(SEED_DIR "/seed.out");
	return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
