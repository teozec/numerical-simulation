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

	// Write info about number of blocks and throws into info file
	ofstream out("data/1.1-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.1-info.dat" << endl;
		return 1;
	}
	out << N_blocks << endl << N_throws_per_block << endl;
	out.close();

	// Open data file
	out.open("data/1.1.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.1.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0., sum_var = 0.;
		int n[N_blocks] = { 0 };

		for (int j = 0; j < N_throws_per_block; j++) {
			// Generate x \in [0, 1)
			double x = rnd.rannyu();

			// Increment the total and the sum of square differences
			sum += x;
			sum_var += (x - 0.5) * (x - 0.5);

			// Increment by one n[i] if x \in [i/N_blocks, (i+1)/N_blocks)
			n[(int) (x*N_blocks)]++;
		}

		// Calculate the mean and the variance
		double ave = sum / N_throws_per_block;
		double ave_var = sum_var / N_throws_per_block;

		// Calculate the chi squared comparing the expected and the actual distributions
		double chi2 = 0.;
		for (int j = 0; j < N_blocks; j++)
			chi2 += (n[j] - N_throws_per_block/N_blocks) *
				(n[j] - N_throws_per_block/N_blocks);
		chi2 *= ((double) N_blocks / N_throws_per_block);

		// Write results to file
		out << ave << '\t' << ave_var << '\t' << chi2 << endl;
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
