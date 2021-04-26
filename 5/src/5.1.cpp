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
#define PSI_100_STEP 1.227
#define PSI_100_SIGMA 0.759
#define PSI_210_STEP 2.977
#define PSI_210_SIGMA 1.876

using namespace std;

// Probability density |psi_100|^2
double psi_100(double x[], int ndim)
{
	double r = 0.;
	for (int i = 0; i < ndim; i++)
		r += x[i] * x[i];
	r = sqrt(r);
	return exp(-2.*r);
}

// Probability density |psi_210|^2
double psi_210(double x[], int ndim)
{
	double r = 0.;
	for (int i = 0; i < ndim; i++)
		r += x[i] * x[i];
	r = sqrt(r);
	double z = x[ndim - 1];
	return z * z * exp(-r);
}
	
int main()
{
	// Construct new random number generator
	Random rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in");

	// Write info about number of blocks and throws in info file
	ofstream info("data/5.1-info.dat");
	if (!info.is_open()) {
		cerr << "Could not open data/5.1-info.dat\n";
		return 1;
	}
	info << N_blocks << '\n' << N_throws_per_block << endl;

	// psi100 with uniform Metropolis
	ofstream out_ave("data/5.1-ave-psi100-unif.dat");
	if (!out_ave.is_open()) {
		cerr << "Could not open data/5.1-ave-psi100-unif.dat\n";
		return 1;
	}
	ofstream out("data/5.1-psi100-unif.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/5.1-psi100-unif.dat\n";
		return 1;
	}

	double xn[3] = {1.5, 1.5, 1.5};
	int accepted = 0;
	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			accepted += rnd.metropolis_unif(xn, 3, PSI_100_STEP, psi_100);
			sum += sqrt(xn[0]*xn[0] + xn[1]*xn[1] + xn[2]*xn[2]);
			if (j % 100 == 0)
				out << xn[0] << '\t' << xn[1] << '\t' << xn[2] << endl;
		}
		double ave = sum / N_throws_per_block;
		out_ave << ave << endl;
	}
	out.close();
	out_ave.close();
	info << (double) accepted / (N_blocks * N_throws_per_block) << endl;

	// psi_100 with gaussian Metropolis
	out_ave.open("data/5.1-ave-psi100-gauss.dat");
	if (!out_ave.is_open()) {
		cerr << "Could not open data/5.1-ave-psi100-gauss.dat\n";
		return 1;
	}
	out.open("data/5.1-psi100-gauss.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/5.1-psi100-gauss.dat\n";
		return 1;
	}

	for (int i = 0; i < 3; i++)
		xn[i] = 1.5;
	accepted = 0;
	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			accepted += rnd.metropolis_gauss(xn, 3, PSI_100_SIGMA, psi_100);
			sum += sqrt(xn[0]*xn[0] + xn[1]*xn[1] + xn[2]*xn[2]);
			if (j % 100 == 0)
				out << xn[0] << '\t' << xn[1] << '\t' << xn[2] << endl;
		}
		double ave = sum / N_throws_per_block;
		out_ave << ave << endl;
	}
	out.close();
	out_ave.close();
	info << (double) accepted / (N_blocks * N_throws_per_block) << endl;


	// psi_210 with uniform Metropolis
	out_ave.open("data/5.1-ave-psi210-unif.dat");
	if (!out_ave.is_open()) {
		cerr << "Could not open data/5.1-ave-psi210-unif.dat\n";
		return 1;
	}
	out.open("data/5.1-psi210-unif.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/5.1-psi210-unif.dat\n";
		return 1;
	}

	accepted = 0;
	for (int i = 0; i < 3; i++)
		xn[i] = 1.5;

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			accepted += rnd.metropolis_unif(xn, 3, PSI_210_STEP, psi_210);
			sum += sqrt(xn[0]*xn[0] + xn[1]*xn[1] + xn[2]*xn[2]);
			if (j % 100 == 0)
				out << xn[0] << '\t' << xn[1] << '\t' << xn[2] << endl;
		}
		double ave = sum / N_throws_per_block;
		out_ave << ave << endl;
	}
	out.close();
	out_ave.close();
	info << (double) accepted / (N_blocks * N_throws_per_block) << endl;

	// psi_210 with gaussian Metropolis
	out_ave.open("data/5.1-ave-psi210-gauss.dat");
	if (!out_ave.is_open()) {
		cerr << "Could not open data/5.1-ave-psi210-gauss.dat\n";
		return 1;
	}
	out.open("data/5.1-psi210-gauss.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/5.1-psi210-gauss.dat\n";
		return 1;
	}

	accepted = 0;
	for (int i = 0; i < 3; i++)
		xn[i] = 1.5;

	for (int i = 0; i < N_blocks; i++) {
		double sum = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			accepted += rnd.metropolis_gauss(xn, 3, PSI_210_SIGMA, psi_210);
			sum += sqrt(xn[0]*xn[0] + xn[1]*xn[1] + xn[2]*xn[2]);
			if (j % 100 == 0)
				out << xn[0] << '\t' << xn[1] << '\t' << xn[2] << endl;
		}
		double ave = sum / N_throws_per_block;
		out_ave << ave << endl;
	}
	out.close();
	out_ave.close();
	info << (double) accepted / (N_blocks * N_throws_per_block) << endl;
	info.close();

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
