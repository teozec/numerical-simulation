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
#include "monte_carlo.h"

using namespace std;

int main()
{
	const int N_steps = 500000;
	ofstream out;

	Montecarlo sim("config/input.dat", "config/config.0");
	out.open("data/7.1-measures.dat");
	if (!out.is_open()) {
		cerr << "Could not open data/7.1-measures.dat\n";
		return 1;
	}

	for (int i = 0; i < N_steps; i++) {
		if ((i+1) % 1000 == 0)
			cout << '.' << flush;
		sim.move();			//Move particles with Verlet algorithm
		sim.measure();
		out << sim.energy << '\t' <<	//Potential energy per particle
			sim.pressure << endl;	//Pressure
	}
	cout << sim.accepted / sim.attempted << endl;
	out.close();

	return 0;
}
