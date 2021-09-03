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
#include "monte_carlo.h"

using namespace std;

Montecarlo::Montecarlo(string input_file, string config_file):
	rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in")
{
	ifstream ReadInput,ReadConf;

	cout << "Classic Lennard-Jones fluid        " << endl;
	cout << "Monte Carlo simulation             " << endl << endl;
	cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl << endl;
	cout << "Boltzmann weight exp(- beta * sum_{i<j} v(r_ij) ), beta = 1/T " << endl << endl;
	cout << "The program uses Lennard-Jones units " << endl;

//Read input informations
	ReadInput.open(input_file);

	ReadInput >> temp;
	beta = 1.0/temp;
	cout << "Temperature = " << temp << endl;

	ReadInput >> npart;
	cout << "Number of particles = " << npart << endl;

	ReadInput >> rho;
	cout << "Density of particles = " << rho << endl;
	vol = (double)npart/rho;
	box = pow(vol,1.0/3.0);
	cout << "Volume of the simulation box = " << vol << endl;
	cout << "Edge of the simulation box = " << box << endl;

	ReadInput >> rcut;
	cout << "Cutoff of the interatomic potential = " << rcut << endl << endl;
    
  //Tail corrections for potential energy and pressure
	vtail = (8.0*pi*rho)/(9.0*pow(rcut,9)) - (8.0*pi*rho)/(3.0*pow(rcut,3));
	ptail = (32.0*pi*rho)/(9.0*pow(rcut,9)) - (16.0*pi*rho)/(3.0*pow(rcut,3));
	cout << "Tail correction for the potential energy = " << vtail << endl;
	cout << "Tail correction for the virial           = " << ptail << endl; 

	ReadInput >> delta;

	ReadInput >> nblk;

	ReadInput >> nstep;

	ReadInput >> nbins;

	cout << "The program perform Metropolis moves with uniform translations" << endl;
	cout << "Moves parameter = " << delta << endl;
	ReadInput.close();


//Prepare arrays for measurements
	iv = 0; //Potential energy
	iw = 1; //Virial
 
	n_props = 2; //Number of observables

//measurement of g(r)
	igofr = 2;
	n_props = n_props + nbins;
	bin_size = (box/2.0)/(double)nbins;

//Read initial configuration
	cout << "Read initial configuration from file config.0 " << endl << endl;
	ReadConf.open(config_file);
	for (int i=0; i<npart; ++i)
	{
		ReadConf >> x[i] >> y[i] >> z[i];
		x[i] = Pbc( x[i] * box );
		y[i] = Pbc( y[i] * box );
		z[i] = Pbc( z[i] * box );
	}
	ReadConf.close();
  
//Evaluate potential energy and virial of the initial configuration
	measure();

//Print	initial values for the potential energy and virial
	cout << "Initial potential energy (with tail corrections) = " << walker[iv]/(double)npart + vtail << endl;
	cout << "Virial                   (with tail corrections) = " << walker[iw]/(double)npart + ptail << endl;
	cout << "Pressure                 (with tail corrections) = " << rho * temp + (walker[iw] + (double)npart * ptail) / vol << endl << endl;
}


void Montecarlo::move()
{
	int o;
	double p, energy_old, energy_new;
	double xold, yold, zold, xnew, ynew, znew;


	for(int i=0; i<npart; i++)
	{
		//Select randomly a particle (for C++ syntax, 0 <= o <= npart-1)
		o = (int)(rnd.rannyu()*npart);

		//Old
		xold = x[o];
		yold = y[o];
		zold = z[o];

		energy_old = boltzmann(xold,yold,zold,o);

		//New
		xnew = Pbc( x[o] + delta*(rnd.rannyu() - 0.5) );
		ynew = Pbc( y[o] + delta*(rnd.rannyu() - 0.5) );
		znew = Pbc( z[o] + delta*(rnd.rannyu() - 0.5) );

		energy_new = boltzmann(xnew,ynew,znew,o);

		//Metropolis test
		p = exp(beta*(energy_old-energy_new));
		if(p >= rnd.rannyu())  
    		{
			//Update
			x[o] = xnew;
			y[o] = ynew;
			z[o] = znew;
    
			accepted = accepted + 1.0;
		}
		attempted = attempted + 1.0;
	}
}

double Montecarlo::boltzmann(double xx, double yy, double zz, int ip)
{
	double ene=0.0;
	double dx, dy, dz, dr;

	for (int i=0; i<npart; ++i) {
		if(i != ip) {
// distance ip-i in pbc
			dx = Pbc(xx - x[i]);
			dy = Pbc(yy - y[i]);
			dz = Pbc(zz - z[i]);

			dr = dx*dx + dy*dy + dz*dz;
			dr = sqrt(dr);

			if(dr < rcut) {
				ene += 1.0/pow(dr,12) - 1.0/pow(dr,6);
			}
		}
	}

	return 4.0*ene;
}

void Montecarlo::measure()
{
	double v = 0.0, w = 0.0;
	double vij, wij;
	double dx, dy, dz, dr;

	//cycle over pairs of particles
	for (int i=0; i<npart-1; ++i) {
		for (int j=i+1; j<npart; ++j) {
			// distance i-j in pbc
			dx = Pbc(x[i] - x[j]);
			dy = Pbc(y[i] - y[j]);
			dz = Pbc(z[i] - z[j]);

			dr = dx*dx + dy*dy + dz*dz;
			dr = sqrt(dr);

			//update of the histogram of g(r)
			if (dr < box / 2.)
				walker[igofr + (int) (2 * dr/box * nbins)] += 2;

			if(dr < rcut) {
				vij = 1.0/pow(dr,12) - 1.0/pow(dr,6);
				wij = 1.0/pow(dr,12) - 0.5/pow(dr,6);

				// contribution to energy and virial
				v += vij;
				w += wij;
			}
		}
	}

	energy = 4. * v;
	pressure = 48. * w / 3.;
	walker[iv] = 4.0 * v;
	walker[iw] = 48.0 * w / 3.0;
}

void Montecarlo::add_tails(double &energy, double &pressure)
{
	energy = energy / (n_throws * npart) + vtail;
	pressure = rho * temp + (pressure / n_throws + ptail * npart) / vol;
}

void Montecarlo::update_gofr()
{
	for (int i = 0; i < nbins; i++) {
		double bin_size = (box/2.0)/(double)nbins;
		double r = i * bin_size;
		double deltaV = 4. * M_PI/3. * (pow((r+bin_size), 3) - pow(r, 3));
		double norm = rho * npart * deltaV * n_throws;
		walker[igofr + i] /= norm;
	}
}

void Montecarlo::ConfFinal(void)
{
  ofstream WriteConf;

  cout << "Print final configuration to file config.final " << endl << endl;
  WriteConf.open("config.final");
  for (int i=0; i<npart; ++i)
  {
    WriteConf << x[i]/box << "   " <<  y[i]/box << "   " << z[i]/box << endl;
  }
  WriteConf.close();

  rnd.save_seed(SEED_DIR "/seed.out");
}

void Montecarlo::ConfXYZ(int nconf){ //Write configuration in .xyz format
  ofstream WriteXYZ;

  WriteXYZ.open("frames/config_" + to_string(nconf) + ".xyz");
  WriteXYZ << npart << endl;
  WriteXYZ << "This is only a comment!" << endl;
  for (int i=0; i<npart; ++i){
    WriteXYZ << "LJ  " << Pbc(x[i]) << "   " <<  Pbc(y[i]) << "   " << Pbc(z[i]) << endl;
  }
  WriteXYZ.close();
}

double Montecarlo::Pbc(double r)  //Algorithm for periodic boundary conditions with side L=box
{
    return r - box * rint(r/box);
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
