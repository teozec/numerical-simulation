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

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0., sum_err = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double x = rnd.rannyu(0., 1.);
			double y = M_PI/2 * cos(M_PI/2 * x);
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

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0., sum_err = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double x = rnd.rannyu();
			x = 1 - sqrt(1-x);
			double y = M_PI/2 * cos(M_PI/2 * x) / (2 * (1-x));
			sum += y;
			sum_err += (y - 1.) * (y - 1.);
		}
		double ave = sum / N_throws_per_block;
		double ave_err = sum_err / N_throws_per_block;
		out << ave << '\t' << ave_err << endl;
	}
	out.close();

//	out.open("data/2.1.c.dat");
//	if (!out.is_open()) {
//		cerr << "Could not open 2.1.dat\n";
//		return 1;
//	}

//	for (int i = 0; i < N_blocks; i++) {
//		double sum = 0., sum_err = 0., ave, ave_err;
//		for (int j = 0; j < N_throws_per_block; j++) {
//			double x;
//			double y;
//			do {
//				x = rnd.rannyu();
//				y = rnd.rannyu(0., 80.8948);
//			} while (384.-48.*M_PI*M_PI*x*x+5*M_PI*M_PI*M_PI*M_PI*x*x*x*x < y);
//			y = M_PI/2 * cos(x*M_PI/2) * (1920-80*M_PI*M_PI+M_PI*M_PI*M_PI*M_PI) / 5.;
//			y /= (384-48*M_PI*M_PI*x*x+M_PI*M_PI*M_PI*M_PI*x*x*x*x);
//			sum += y;
//			sum_err += (y - 1.) * (y - 1.);
//		}
//		ave = sum / N_throws_per_block;
//		ave_err = sum_err / N_throws_per_block;
//	}
//	out.close();

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
