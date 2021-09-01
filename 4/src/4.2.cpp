/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 4.2
// Molecular dynamics simulation with block mean.

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
	const int N_steps_per_block = 100;
	const int N_blocks = 100;
	string old;
	ofstream out;
	bool measure, equilibrate;

	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " start|equilibrate|measure" << endl;
		return 1;
	} else if (string(argv[1]) == "start") {
		old = "";
		measure = false;
		equilibrate = false;
	} else if (string(argv[1]) == "equilibrate") {
		old = "config/4.2-old.0";
		measure = false;
		equilibrate = true;
	} else if (string(argv[1]) == "measure") {
		old = "config/4.2-old.0";
		measure = true;
		equilibrate = false;
	} else {
		cerr << "Usage: " << argv[0] << " start|equilibrate|measure" << endl;
		return 1;
	}

	MolDyn sim = MolDyn("config/4.2-input.dat", "config/4.2-config.0", old);             //Inizialization
	if (string(argv[1]) == "start")
		sim.info();

	if (equilibrate)
		sim.rescale();
	ofstream ave;
	if (measure) {
		ave.open("data/4.2-averages.dat", ios::app);
		if (!ave.is_open()) {
			cerr << "Could not open data/4.2-averages.dat\n";
			return 1;
		}
	}
	out.open("data/4.2-measures.dat", ios::app);
	if (!out.is_open()) {
		cerr << "Could not open data/4.2-measures.dat\n";
		return 1;
	}

	for (int i = 0; i < N_blocks; i++) {
		double ave_pot = 0., ave_kin = 0., ave_temp = 0., ave_etot = 0.;
		for (int j = 0; j < N_steps_per_block; j++) {
			sim.move();           //Move particles with Verlet algorithm
			if ((i*N_steps_per_block + j + 1) % 1000 == 0)
				cout << '.' << flush;
			if ((i*N_steps_per_block + j + 1) % 10 == 0) {
				sim.measure();
				out << sim.measured_epot << '\t' <<	//Potential energy per particle
					sim.measured_ekin << '\t' <<	//Kinetic energy per particle
					sim.measured_temp << '\t' <<	//Temperature
					sim.measured_etot << endl;	//Total energy per particle
			}
			if (measure) {
				sim.measure();     //Properties measurement
				ave_pot += sim.measured_epot; //Potential energy per particle
				ave_kin += sim.measured_ekin; //Kinetic energy per particle
				ave_temp += sim.measured_temp; //Temperature
				ave_etot += sim.measured_etot; //Total energy per particle
			}
		}

		if (measure) {
			ave_pot /= N_steps_per_block;
			ave_kin /= N_steps_per_block;
			ave_temp /= N_steps_per_block;
			ave_etot /= N_steps_per_block;
			ave << ave_pot << '\t' << ave_kin << '\t' << ave_temp << '\t' << ave_etot << endl;
		}
	}
	cout << endl;
	out.close();
	ave.close();

	sim.conf_final("config/4.2-config.final", "config/4.2-old.final");         //Write final configuration to restart

	return 0;
}
