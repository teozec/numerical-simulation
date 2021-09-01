/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 2.1
// Montecarlo intevration via uniform and importance sampling.
// \int_0^1 f(x) dx, with f(x) = pi/2 * cos(pi/2 * x)

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

int main()
{
	const int N_blocks = 100;
	const int N_throws_per_block = 10000;

	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of blocks and throws in info file
	ofstream out("data/2.1-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/2.1-info.dat\n";
		return 1;
	}
	out << N_blocks << '\n' << N_throws_per_block << endl;
	out.close();

	// Open data file
	out.open("data/2.1-unif.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/2.1-unif.dat\n";
		return 1;
	}

	// Compute integral sampling from uniform distribution
	for (int i = 0; i < N_blocks; i++) {
		double sum = 0., sum_err = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double x = rnd.rannyu(0., 1.);		// p(x) = 1
			double y = M_PI/2 * cos(M_PI/2 * x);	// g(x) = f(x)
			sum += y;
			sum_err += (y - 1.) * (y - 1.);
		}
		double ave = sum / N_throws_per_block;
		double ave_err = sum_err / N_throws_per_block;
		out << ave << '\t' << ave_err << endl;
	}
	out.close();

	out.open("data/2.1-importance.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/2.1-importance.dat\n";
		return 1;
	}

	// Compute integral sampling from p(x) = 2*(1-x) (Taylor expansion of f(x) in x=1)
	for (int i = 0; i < N_blocks; i++) {
		double sum = 0., sum_err = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double x = rnd.rannyu();				// p(x) = 2 * (1 - x)
			x = 1 - sqrt(1-x);					// Inverse cumulative: x(y) = 1 - sqrt(1 - y), with p(y) = 1
			double y = M_PI/2 * cos(M_PI/2 * x) / (2 * (1-x));	// g(x) = f(x) / p(x)
			sum += y;
			sum_err += (y - 1.) * (y - 1.);
		}
		double ave = sum / N_throws_per_block;
		double ave_err = sum_err / N_throws_per_block;
		out << ave << '\t' << ave_err << endl;
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
