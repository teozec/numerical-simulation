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
#define N_walks_per_block 1000
#define N_steps_per_walk 10000

using namespace std;

int main()
{
	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of blocks and throws in info file
	ofstream out("data/2.2-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/2.2-info.dat\n";
		return 1;
	}
	out << N_blocks << endl << N_walks_per_block << endl << N_steps_per_walk << endl;
	out.close();

	// Open data file
	out.open("data/2.2-disc.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/2.2-disc.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_walks_per_block; j++) {
			int r[3] = { 0 };
			for (int z = 0; z < N_steps_per_walk; z++) {
				int axis = (int) rnd.rannyu(0., 3.);
				double dir = rnd.rannyu(-1., 1.);
				r[axis] += dir >= 0. ? 1 : -1;
			}
			sum += sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
		}
		double ave = sum / N_walks_per_block;
		out << ave << endl;
	}
	out.close();

	// Open data file
	out.open("data/2.2-cont.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/2.2-disc.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_walks_per_block; j++) {
			double r[3] = { 0 };
			for (int z = 0; z < N_steps_per_walk; z++) {
				double phi = rnd.rannyu(0, 2*M_PI);
				double theta = acos(1 - 2*rnd.rannyu());
				r[0] += sin(theta) * cos(phi);
				r[1] += sin(theta) * sin(phi);
				r[2] += cos(theta);
			}
			sum += sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
		}
		double ave = sum / N_walks_per_block;
		out << ave << endl;
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
