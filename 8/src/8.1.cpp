/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 8.1
// Variational Montecarlo

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "random.h"

#define N_blocks 100
#define N_throws_per_block 100000
#define PSI_100_STEP 1.227
#define PSI_100_SIGMA 0.759
#define PSI_210_STEP 2.977
#define PSI_210_SIGMA 1.876

using namespace std;

double psi_T(double x, double mu, double sigma);
double d2_psi_T(double x, double mu, double sigma);
double potential(double x);

int main()
{
	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of blocks and throws in info file
	ofstream info("data/8.1-info.dat");
	if (!info.is_open()) {
		cerr << "Could not open data/8.1-info.dat\n";
		return 1;
	}
	info << N_blocks << '\n' << N_throws_per_block << endl;

	ofstream out("data/8.1-data.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/8.1-data.dat\n";
		return 1;
	}

	double x = 1.5;
	const double delta = 2.3;
	double mu = .5, sigma = 1.;
	auto psi_T_2 = [mu, sigma](double x[], int ndim) {
			double p = psi_T(*x, mu, sigma);
			return p * p;
	};
	int accepted = 0;
	for (int i = 0; i < N_blocks; i++) {
		double H = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			accepted += rnd.metropolis_unif(&x, 1, delta, psi_T_2);
			H += -0.5 * d2_psi_T(x, mu, sigma) / psi_T(x, mu, sigma) + potential(x);
		}
		double ave = H / N_throws_per_block;
		out << ave << endl;
		if ((i+1) % 2 == 0)
			cout << '.' << flush;
	}
	out.close();
	info << (double) accepted / (N_blocks * N_throws_per_block) << endl;

	cout << endl;

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
	phi_plus *= phi_plus;

	return (phi_minus_2 - 1.) * exp(-0.5 * phi_minus_2) +
		(phi_plus_2 - 1.) * exp(-0.5 * phi_plus_2);
}

/*
double dmu_psi_T(double x, double mu, double sigma)
{
	double phi_minus = (x - mu) / sigma;
	double phi_minus_2 = phi_minus * phi_minus;
	double phi_plus = (x + mu) / sigma;
	double phi_plus_2 = phi_plus * phi_plus;

	return (phi_minus * exp(-0.5 * phi_minus_2) - phi_plus * exp(-0.5 * phi_plus_2)) / sigma;
}

double dsigma_psi_T(double x, double mu, double sigma)
{
	double phi_minus = (x - mu) / sigma;
	double phi_minus_2 = phi_minus * phi_minus;
	double phi_plus = (x + mu) / sigma;
	double phi_plus_2 = phi_plus * phi_plus;

	return (phi_minus_2 * exp(-0.5 * phi_minus_2) + phi_plus_2 * exp(-0.5 * phi_plus_2)) / sigma;
}

double dmu_d2_psi_T(double x, double mu, double sigma)
{
	double phi_minus = (x - mu) / sigma;
	double phi_minus_2 = phi_minus * phi_minus;
	double xi_minus = -1 - phi_minus + phi_minus_2;
	double phi_plus = (x + mu) / sigma;
	double phi_plus_2 = phi_plus * phi_plus;
	double xi_plus = -1 - phi_plus + phi_plus_2;

	return (xi_minus * exp(-0.5 * phi_minus_2) - xi_plus * exp(-0.5 * phi_plus_2)) / sigma;
}

double dsigma_d2_psi_T(double x, double mu, double sigma)
{
	double phi_minus = (x - mu) / sigma;
	double phi_minus_2 = phi_minus * phi_minus;
	double xi_minus = 1 - phi_minus + phi_minus_2;
	double phi_plus = (x + mu) / sigma;
	double phi_plus_2 = phi_plus * phi_plus;
	double xi_plus = 1 - phi_plus + phi_plus_2;

	return (xi_minus * phi_minus * exp(-0.5 * phi_minus_2) + xi_plus * phi_plus * exp(-0.5 * phi_plus_2)) / sigma;
}

double dmu_H(double x, double mu, double sigma)
{
	double psi = psi_T(x, mu, sigma);
	return dmu_d2_psi_T / psi + dmu_psi_T * d2_psi_T / (psi * psi);
}

double dsigma_H(double x, double mu, double sigma)
{
	double psi = psi_T(x, mu, sigma);
	return dsigma_d2_psi_T / psi + dsigma_psi_T * d2_psi_T / (psi * psi);
}
*/

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
