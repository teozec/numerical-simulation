/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 1.3
// Estimate pi using Buffon's method.

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
	const double d = 1.0;	// The distance between lines
	const double l = 0.5;	// The length of the needle

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
			// Generate the x position of the first vertex of the needle
			double x1 = rnd.rannyu(0., d);


			// Sample a point from the quarter of circle r<=1, x>0, y>0
			double x, y;
			do {
				x = rnd.rannyu();
				y = rnd.rannyu();
			} while (x*x + y*y > 1.);

			// Use the sampled point (x,y) to sample an angle theta,
			// 	uniformely distributed in the (0,pi/2) range
			double theta = acos(x/sqrt(x*x+y*y));
			double x2 = x1 + l*sin(theta);

			// If the needle crosses the line, increment the number of hits
			if (x2 > d)
				n_hit++;
		}

		// Estimate pi inverting the formula for the probabiilty
		// 	that the needle intersects the line
		double pi = 2*l*N_throws_per_block / (n_hit * d);
		out << pi << endl;
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
