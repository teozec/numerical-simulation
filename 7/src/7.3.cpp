/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <cstdlib>	// srand, rand: to generate random number
#include <iostream>	// cin, cout: Standard Input/Output Streams Library
#include <fstream>	// Stream class to both read and write from/to files.
#include <cmath>	// rint, pow
#include <string>
#include <iomanip>
#include "nve.h"

using namespace std;

int main()
{
	const int N_blocks = 10, N_throws_per_block = 1000;
	ofstream out;

	NVE sim("config/7.3-input.dat", "config/config.fcc");
	out.open("data/7.3-gofr.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/7.3-gofr.dat\n";
		return 1;
	}
	sim.info();
	sim.n_throws = N_throws_per_block;

	for (int i = 0; i < N_blocks; i++) {
		cout << '.' << flush;
		for (int j = 0; j < N_throws_per_block; j++) {
			sim.move();           //Move particles with Verlet algorithm
			sim.measure();
		}
		sim.update_gofr();
		for (int j = 0; j < sim.nbins; j++) {
			out << sim.gofr[j] << '\t';
			sim.gofr[j] = 0.;
		}
		out << endl;
	}
	cout << endl;
	out.close();

	return 0;
}
