/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 6.1
// Ising model.

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "ising.h"

#define N_equilib 1000000
#define N_blocks 100
#define N_throws_per_block 10000

#define N_spins 50
#define J 1.
#define h 0.02

using namespace std;

int main()
{ 
	for (int t = 0; t < 20; t++) {
		double temp = 0.5 + t * 1.5 / 20.;
		Ising metro0(false, N_spins, temp, J, 0.);
		Ising metroh(false, N_spins, temp, J, h);
		Ising gibbs0(true, N_spins, temp, J, 0.);
		Ising gibbsh(true, N_spins, temp, J, h);

		ostringstream metro_filename_stream;
		metro_filename_stream << "data/6.1-metro-" << temp << ".dat";
		string metro_filename = metro_filename_stream.str();
		ofstream metro_out(metro_filename);
		if (!metro_out.is_open()) {
			cerr << "Could not open " << metro_filename << endl;
			return 1;
		}

		ostringstream gibbs_filename_stream;
		gibbs_filename_stream << "data/6.1-gibbs-" << temp << ".dat";
		string gibbs_filename = gibbs_filename_stream.str();
		ofstream gibbs_out(gibbs_filename);
		if (!gibbs_out.is_open()) {
			cerr << "Could not open " << gibbs_filename << endl;
			return 1;
		}

		for (int i = 0; i < N_equilib; i++) {
			metro0.move();
			metroh.move();
			gibbs0.move();
			gibbsh.move();
		}

		for (int i = 0; i < N_blocks; i++) {
			double metro_internal_energy = 0.;
			double metro_internal_energy2 = 0.;
			double metro_magnetization = 0.;
			double metro_susceptivity = 0.;
			double gibbs_internal_energy = 0.;
			double gibbs_internal_energy2 = 0.;
			double gibbs_magnetization = 0.;
			double gibbs_susceptivity = 0.;

			for (int j = 0; j < N_throws_per_block; j++) {
				metro0.move();
				metro0.measure();
				metro_internal_energy += metro0.internal_energy;
				metro_internal_energy2 += metro0.internal_energy2;
				metro_susceptivity += metro0.susceptivity;

				metroh.move();
				metroh.measure();
				metro_magnetization += metroh.magnetization;

				gibbs0.move();
				gibbs0.measure();
				gibbs_internal_energy += gibbs0.internal_energy;
				gibbs_internal_energy2 += gibbs0.internal_energy2;
				gibbs_susceptivity += gibbs0.susceptivity;

				gibbsh.move();
				gibbsh.measure();
				gibbs_magnetization += gibbsh.magnetization;
			}
			metro_out << metro_internal_energy / N_throws_per_block << '\t' <<
				metro_internal_energy2 / N_throws_per_block << '\t' <<
				metro_magnetization / N_throws_per_block << '\t' <<
				metro_susceptivity / N_throws_per_block << endl;

			gibbs_out << gibbs_internal_energy / N_throws_per_block << '\t' <<
				gibbs_internal_energy2 / N_throws_per_block << '\t' <<
				gibbs_magnetization / N_throws_per_block << '\t' <<
				gibbs_susceptivity / N_throws_per_block << endl;
		}
		cout << '.' << flush;
		metro_out.close();
		gibbs_out.close();
	}
	cout << endl;

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
