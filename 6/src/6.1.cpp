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
		Ising ising0(false, N_spins, temp, J, 0.);	//Inizialization
		Ising isingh(false, N_spins, temp, J, h);	//Inizialization

		ostringstream filename_stream;
		filename_stream << "data/6.1-gibbs-" << temp << ".dat";
		string filename = filename_stream.str();
		ofstream out(filename);
		if (!out.is_open()) {
			cerr << "Could not open " << filename << endl;
			return 1;
		}

		for (int i = 0; i < N_equilib; i++) {
			ising0.move();
			isingh.move();
		}

		for (int i = 0; i < N_blocks; i++) {
			double internal_energy = 0.;
			double internal_energy2 = 0.;
			double magnetization = 0.;
			double susceptivity = 0.;

			for (int j = 0; j < N_throws_per_block; j++) {
				ising0.move();
				ising0.measure();
				internal_energy += ising0.internal_energy;
				internal_energy2 += ising0.internal_energy2;
				susceptivity += ising0.susceptivity;

				isingh.move();
				isingh.measure();
				magnetization += isingh.magnetization;
			}
			out << internal_energy / N_throws_per_block << '\t' <<
				internal_energy2 / N_throws_per_block << '\t' <<
				magnetization / N_throws_per_block << '\t' <<
				susceptivity / N_throws_per_block << endl;
		}
		cout << '.' << flush;
		out.close();
		//ising.ConfFinal(); //Write final configuration
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
