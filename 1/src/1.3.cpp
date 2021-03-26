/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "random.h"

#define N_blocks 100
#define N_throws_per_block 10000
#define d 1.0
#define l 0.5

using namespace std;

int main()
{
	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of blocks and throws in info file
	ofstream out("data/1.3-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.3-info.dat\n";
		return 1;
	}
	out << N_blocks << '\n' << N_throws_per_block << endl;
	out.close();

	out.open("data/1.3.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.3.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		int n_hit = 0;

		for (int j = 0; j < N_throws_per_block; j++) {
			double x1 = rnd.rannyu(0., d);
			double x, y;

			do {
				x = rnd.rannyu();
				y = rnd.rannyu();
			} while (x*x + y*y > 1.);

			double theta = y >= 0. ?
				acos(x/sqrt(x*x+y*y)) : 2*M_PI - acos(x/sqrt(x*x+y*y));
			double x2 = x1 + l*sin(theta);

			if (x2 < 0 or x2 > d)
				n_hit++;
		}

		double pi = 2*l*N_throws_per_block / (n_hit * d);
		out << pi << endl;
	}

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
