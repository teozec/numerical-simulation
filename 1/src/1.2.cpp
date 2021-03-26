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

#define N_throws 10000

using namespace std;

int main()
{
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	ofstream out("data/1.2-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.2-info.dat\n";
		return 1;
	}
	out << N_throws << endl;
	out.close();

	out.open("data/1.2-unif.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/1.2-unif.dat\n";
		return 1;
	}

	const int N_sums[4] = {1, 2, 10, 100};
	const char end_c[4] = {'\t', '\t', '\t', '\n'};

	for (int i = 0; i < N_throws; i++) {
		for (int j = 0; j < 4; j++) {
			int s = 0;
			for (int z = 0; z < N_sums[j]; z++)
				s += (int) rnd.rannyu(1., 7.);
			out << s/N_sums[j] << end_c[j];
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
