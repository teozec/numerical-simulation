/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 2.2
// 3D discrete and continuous random walk.

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "random.h"

#define N_blocks 100
#define N_walks_per_block 1000
#define N_steps_per_walk 100

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

	// Discrete random walk
	for (int i = 0; i < N_blocks; i++) {
		double r2[N_steps_per_walk] = { 0. };	// Store the mean square distance at each step
		for (int j = 0; j < N_walks_per_block; j++) {
			int r[3] = { 0 };
			for (int k = 0; k < N_steps_per_walk; k++) {
				int axis = (int) rnd.rannyu(0., 3.);	// Sample x, y or z axis
				double dir = rnd.rannyu(-1., 1.);	// Sample positive or negative direction
				r[axis] += (dir >= 0. ? 1 : -1);		// Step
				r2[k] += r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
			}
		}
		// Write block mean square distance for each step
		for (int j = 0; j < N_steps_per_walk; j++)
			out << sqrt(r2[j] / N_walks_per_block) << '\t';
		out << endl;
	}
	out.close();

	// Open data file
	out.open("data/2.2-cont.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/2.2-disc.dat\n";
		return 1;
	}

	// Continuous random walk
	for (int i = 0; i < N_blocks; i++) {
		double r2[N_steps_per_walk] = { 0. };	// Store the mean square distance at each step
		for (int j = 0; j < N_walks_per_block; j++) {
			double r[3] = { 0 };
			for (int k = 0; k < N_steps_per_walk; k++) {
				// Sample direction
				double phi = rnd.rannyu(0, 2*M_PI);
				double theta = acos(1 - 2*rnd.rannyu());
				r[0] += sin(theta) * cos(phi);
				r[1] += sin(theta) * sin(phi);
				r[2] += cos(theta);
				r2[k] += r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
			}
		}
		// Write block mean square distance for each step
		for (int j = 0; j < N_steps_per_walk; j++)
			out << sqrt(r2[j] / N_walks_per_block) << '\t';
		out << endl;
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
