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

using namespace std;

int main()
{
	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of blocks and throws in info file
	ofstream out("data/1.1-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.1-info.dat\n";
		return 1;
	}
	out << N_blocks << '\n' << N_throws_per_block << endl;
	out.close();

	// Open data file
	out.open("data/1.1.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.1.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0., sum_err = 0.;
		int n[N_blocks] = { 0 };

		for (int j = 0; j < N_throws_per_block; j++) {
			double x = rnd.rannyu();
			sum += x;
			sum_err += (x - 0.5) * (x - 0.5);
			n[(int) (x*N_blocks)]++;
		}

		double ave = sum / N_throws_per_block;
		double ave_err = sum_err / N_throws_per_block;

		double chi2 = 0.;
		for (int j = 0; j < N_blocks; j++)
			chi2 += (n[j] - N_throws_per_block/N_blocks) *
				(n[j] - N_throws_per_block/N_blocks);
		chi2 *= ((double) N_blocks / N_throws_per_block);

		out << ave << '\t' << ave_err << '\t' << chi2 << endl;
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
