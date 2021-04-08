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
#define N_steps_per_throw 100

using namespace std;

int main()
{
	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of blocks and throws in info file
	ofstream out("data/3.1-info.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/3.1-info.dat\n";
		return 1;
	}
	out << N_blocks << '\n' << N_throws_per_block << '\n' << N_steps_per_throw << endl;
	out.close();

	const double S0 = 100., T = 1., K = 100., r = 0.1, sigma = 0.25;

	// Open data file
	out.open("data/3.1-call-direct.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/3.1-call-direct.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double W = rnd.gauss(0., T);
			double ST = S0*exp((r - sigma*sigma/2.) * T + sigma * W);
			sum += ST > K ? exp(-r*T) * (ST - K) : 0;
		}
		double ave = sum / N_throws_per_block;
		out << ave << endl;
	}
	out.close();

	// Open data file
	out.open("data/3.1-call-disc.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/3.1-call-disc.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double dt = T / N_steps_per_throw;
			double sdt = sqrt(dt);
			double ST = S0;
			for (int k = 0; k < N_steps_per_throw; k++) {
				double Z = rnd.gauss(0., 1);
				ST *= exp((r - sigma*sigma/2.) * dt +
					sigma * Z * sdt);
			}
			sum += ST > K ? exp(-r*T) * (ST - K) : 0;
		}
		double ave = sum / N_throws_per_block;
		out << ave << endl;
	}
	out.close();

	// Open data file
	out.open("data/3.1-put-direct.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/3.1-put-direct.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double W = rnd.gauss(0., T);
			double ST = S0*exp((r - sigma*sigma/2.) * T + sigma * W);
			sum += ST < K ? exp(-r*T) * (K - ST) : 0;
		}
		double ave = sum / N_throws_per_block;
		out << ave << endl;
	}
	out.close();

	// Open data file
	out.open("data/3.1-put-disc.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/3.1-call-disc.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			double dt = T / N_steps_per_throw;
			double sdt = sqrt(dt);
			double ST = S0;
			for (int k = 0; k < N_steps_per_throw; k++) {
				double Z = rnd.gauss(0., 1);
				ST *= exp((r - sigma*sigma/2.) * dt +
					sigma * Z * sdt);
			}
			sum += ST < K ? exp(-r*T) * (K - ST) : 0;
		}
		double ave = sum / N_throws_per_block;
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
