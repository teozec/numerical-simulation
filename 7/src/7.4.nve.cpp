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
#include "mol_dyn.h"

using namespace std;

int main(int argc, char *argv[])
{
	const int N_steps_per_block = 500;
	const int N_blocks = 100;
	string old;
	bool measure, equilibrate;
	ofstream out;

	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " start|equilibrate|measure phase" << endl;
		return 1;
	}
	string phase(argv[2]);
	if (string(argv[1]) == "start") {
		old = "";
		measure = false;
		equilibrate = false;
	} else if (string(argv[1]) == "equilibrate") {
		old = "config/7.4-old-" + phase + ".0";
		measure = false;
		equilibrate = true;
	} else if (string(argv[1]) == "measure") {
		old = "config/7.4-old-" + phase + ".0";
		measure = true;
		equilibrate = false;
	} else {
		cerr << "Usage: " << argv[0] << " start|equilibrate|measure phase" << endl;
		return 1;
	}

	MolDyn sim = MolDyn("config/7.4-input-" + phase + ".dat", "config/7.4-config-" + phase + ".0", old);		//Inizialization
	if (string(argv[1]) == "start")
		sim.info();
	if (equilibrate)
		sim.rescale();
	sim.n_throws = N_steps_per_block;

	out.open("data/7.4-nve-" + phase + "-gofr.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/7.4-nve-" + phase + "-gofr.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		cout << '.' << flush;
		for (int j = 0; j < N_steps_per_block; j++) {
			sim.move();           //Move particles with Verlet algorithm
			if (measure) {
				sim.measure();     //Properties measurement
			}
		}

		if (measure) {
			sim.update_gofr();
			for (int j = 0; j < sim.nbins; j++) {
				out << sim.gofr[j] << '\t';
				sim.gofr[j] = 0.;
			}
			out << endl;
		}
	}
	cout << endl;
	out.close();

	sim.conf_final("config/7.4-config-" + phase + ".final", "config/7.4-old-" + phase + ".final");         //Write final configuration to restart

	return 0;
}
