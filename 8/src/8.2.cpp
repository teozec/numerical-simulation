/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 8.2
// Variational Montecarlo: mu and sigma minimization

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

double psi_T(double x, double mu, double sigma);
double d2_psi_T(double x, double mu, double sigma);
double potential(double x);

int main(int argc, char *argv[])
{
	const long N_blocks = 100, N_throws_per_block = 1000, N_fine_throws_per_block = 10000;
	const long N_grid = 50;
	const double start = .5, step = .01;
	int accepted = 0;
	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	double x = 1.5;
	const double delta = 2.3;
	double mu, sigma;

	// Write info about number of blocks and throws in info file
	ofstream info("data/8.2-info.dat");
	if (!info.is_open()) {
		cerr << "Could not open data/8.2-info.dat\n";
		return 1;
	}
	info << N_grid << '\n' << N_blocks << '\n' << N_throws_per_block << '\n' << N_fine_throws_per_block << '\n' << start << '\n' << step << endl;

	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " grid|sample" << endl;
		return 1;
	}
	if (string{argv[1]} == "grid") {
		ofstream out("data/8.2-data.dat");
		if (!out.is_open()) {
			cerr << "Could not open data/8.2-data.dat\n";
			return 1;
		}

	

	for (int i = 0; i < N_grid; i++) {
		cout << '.' << flush;
		mu = start + i * step;
		for (int j = 0; j < N_grid; j++) {
			sigma = start + j * step;
			auto psi_T_2 = [mu, sigma](double x[], int ndim) {
							double p = psi_T(*x, mu, sigma);
							return p * p;
					};
			for (int k = 0; k < N_blocks; k++) {
				double H = 0.;
				for (int l = 0; l < N_throws_per_block; l++) {
					accepted += rnd.metropolis_unif(&x, 1, delta, psi_T_2);
					H += -0.5 * d2_psi_T(x, mu, sigma) / psi_T(x, mu, sigma) + potential(x);
				}
				double ave = H / N_throws_per_block;
				out << ave << '\t';
			}
			out << endl;
		}
	}
		cout << endl << (double) accepted / (N_grid * N_grid * N_blocks * N_throws_per_block) << endl;
	} else if (string{argv[1]} == "sample") {
		// Values that minimize energy, found using Jupyter notebook
		mu = 0.83;
		sigma = 0.61;

		auto psi_T_2 = [mu, sigma](double x[], int ndim) {
						double p = psi_T(*x, mu, sigma);
						return p * p;
				};


		ofstream out{"data/8.2-H.dat"};
		if (!out.is_open()) {
			cerr << "Could not open data/8.2-H.dat\n";
			return 1;
		}
		ofstream sample("data/8.2-sample.dat");
		if (!out.is_open()) {
			cerr << "Could not open data/8.2-sample.dat\n";
			return 1;
		}
		for (int k = 0; k < N_blocks; k++) {
			double H = 0.;
			for (int l = 0; l < N_fine_throws_per_block; l++) {
				accepted += rnd.metropolis_unif(&x, 1, delta, psi_T_2);
				H += -0.5 * d2_psi_T(x, mu, sigma) / psi_T(x, mu, sigma) + potential(x);
				sample << x << endl;
			}
			double ave = H / N_fine_throws_per_block;
			out << ave << endl;
		}
	} else {
		cerr << "Usage: " << argv[0] << " grid|sample" << endl;
		return 1;
	}

	return 0;
}

double psi_T(double x, double mu, double sigma)
{
	double phi_minus = (x - mu) / sigma;
	double phi_plus = (x + mu) / sigma;
	return exp(-0.5 * phi_minus * phi_minus) + exp(-0.5 * phi_plus * phi_plus);
}

double d2_psi_T(double x, double mu, double sigma)
{
	double phi_minus = (x - mu) / sigma;
	double phi_minus_2 = phi_minus * phi_minus;
	double phi_plus = (x + mu) / sigma;
	double phi_plus_2 = phi_plus * phi_plus;

	return ((phi_minus_2 - 1.) * exp(-0.5 * phi_minus_2) +
		(phi_plus_2 - 1.) * exp(-0.5 * phi_plus_2)) / (sigma * sigma);
}

double potential(double x)
{
	return x*x*x*x - 2.5 * x*x;
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
