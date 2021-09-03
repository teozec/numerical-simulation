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
#include "nvt.h"

using namespace std;

int main()
{
	const int N_blocks = 100, N_throws_per_block = 500;
	ofstream out, out_gofr;

	NVT sim("config/nvt-input.dat", "config/config.0");
	out.open("data/7.4-measures.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/7.4-measures.dat\n";
		return 1;
	}
	out_gofr.open("data/7.4-gofr.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/7.4-gofr.dat\n";
		return 1;
	}

	// Equilibrate
	for (int i = 0; i < 5000; i++)
		sim.move();

	sim.n_throws = N_throws_per_block;
	for (int i = 0; i < N_blocks; i++) {
		cout << '.' << flush;
		double energy = 0., pressure = 0.;
		for (int j = 0; j < N_throws_per_block; j++) {
			sim.move();           //Move particles with Verlet algorithm
			sim.measure();
			energy += sim.energy;
			pressure += sim.pressure;
		}
		sim.add_tails(energy, pressure);
		out << energy << '\t' <<	//Potential energy per particle
			pressure << endl;	//Pressure
		sim.update_gofr();
		for (int j = 0; j < sim.nbins; j++) {
			out_gofr << sim.walker[sim.igofr + j] << '\t';
			sim.walker[sim.igofr + j] = 0.;
		}
		out_gofr << endl;
	}

	cout << endl;
	out.close();

	//sim.conf_final("config/4.3-config-" + phase + ".final", "config/4.3-old-" + phase + ".final");         //Write final configuration to restart

	return 0;
}
