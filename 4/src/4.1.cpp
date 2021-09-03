/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

// Exercise 4.1
// Molecular dynamics simulation.

#include <cstdlib>	// srand, rand: to generate random number
#include <iostream>	// cin, cout: Standard Input/Output Streams Library
#include <fstream>	// Stream class to both read and write from/to files.
#include <cmath>	// rint, pow
#include <string>
#include <iomanip>
#include "nve.h"

using namespace std;

int main(int argc, char *argv[])
{
	string old;
	bool equilibrate;
	const int N_steps = 10000;

	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " start|equilibrate|measure" << endl;
		return 1;
	} else if (string(argv[1]) == "start") {
		old = "";
		equilibrate = false;
	} else if (string(argv[1]) == "equilibrate") {
		old = "config/4.1-old.0";
		equilibrate = true;
	} else if (string(argv[1]) == "measure") {
		old = "config/4.1-old.0";
		equilibrate = false;
	} else {
		cerr << "Usage: " << argv[0] << " start|equilibrate|measure" << endl;
		return 1;
	}

	NVE sim = NVE("config/4.1-input.dat", "config/4.1-config.0", old);	//Inizialization
	if (string(argv[1]) == "start")
		sim.info();

	if (equilibrate)
		sim.rescale();

	ofstream out("data/4.1-measures.dat", ios::app);
	if (!out.is_open()) {
		cerr << "Could not open data/4.1-measures.dat\n";
		return 1;
	}

	for (int i = 0; i < N_steps; i++) {
		sim.move();					//Move particles with Verlet algorithm
		if ((i + 1) % 1000 == 0)
			cout << '.' << flush;
		if ((i + 1) % 10 == 0) {
			sim.measure();
			out << sim.measured_epot << '\t' <<	//Potential energy per particle
				sim.measured_ekin << '\t' <<	//Kinetic energy per particle
				sim.measured_temp << '\t' <<	//Temperature
				sim.measured_etot << endl;	//Total energy per particle
		}
	}
	cout << endl;
	out.close();

	sim.conf_final("config/4.1-config.final", "config/4.1-old.final");	//Write final configuration to restart

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
