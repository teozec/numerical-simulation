/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <cstdlib>     // srand, rand: to generate random number
#include <iostream>     // cin, cout: Standard Input/Output Streams Library
#include <fstream>      // Stream class to both read and write from/to files.
#include <cmath>        // rint, pow
#include <string>
#include <iomanip>
#include <cstdlib>
#include "mol_dyn.h"

using namespace std;

void MolDyn::init_input(string input)
{
	ifstream input_file;
	input_file.open(input); //Read input
	if (!input_file.is_open()) {
		cerr << "PROBLEM: Unable to open " << input << endl;
		exit(1);
	}
	input_file >> temp >> npart >> rho >> rcut >> delta >> nstep >> iprint >> nbins;
	vol = (double) npart / rho;
	box = pow(vol, 1.0 / 3.0);
	if (nbins > 0) {
		gofr = new double[nbins];
	} else {
		gofr = NULL;
		nbins = 0;
	}

	input_file.close();
}

void MolDyn::init_config(string config, double r[][3])
{
	ifstream config_file;
	config_file.open(config);
	if (!config_file.is_open()) {
		cerr << "PROBLEM: Unable to open " << config << endl;
		exit(1);
	}
	for (int i = 0; i < npart; i++) {
		for (int coord = 0; coord < 3; coord++) {
			config_file >> r[i][coord];
			r[i][coord] *= box;
		}
	}
	config_file.close();
}

void MolDyn::init_vel()
{
	//Prepare initial velocities
	double sumv[3] = {0.0};
	for (int i = 0; i < npart; i++) {
		for (int coord = 0; coord < 3; coord++) {
			v[i][coord] = rand() / double(RAND_MAX) - 0.5;
			sumv[coord] += v[i][coord];
		}
	}
	for (int coord = 0; coord < 3; coord++)
 		sumv[coord] /= (double) npart;

	double sumv2 = 0.0;
	for (int i = 0; i < npart; i++) {
		for (int coord = 0; coord < 3; coord++)
			v[i][coord] -= sumv[coord];
		sumv2 += v[i][0] * v[i][0] + v[i][1] * v[i][1] + v[i][2] * v[i][2]; // X
	}
	sumv2 /= (double) npart;

	double fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor 
	for (int i = 0; i < npart; i++) {
		for (int coord = 0; coord < 3; coord++) {
			v[i][coord] *= fs;
			r_old[i][coord] = pbc(r[i][coord] - v[i][coord] * delta);
		}
	}
}


//Prepare all stuff for the simulation
MolDyn::MolDyn(string input, string config, string old)
{
	seed = 1;    //Set seed for random numbers
	srand(seed); //Initialize random number generator

	init_input(input);
  
	//r = (double (*)[3]) malloc(npart * sizeof *r);
	r = new double[npart][3];
	//r_old = (double (*)[3]) malloc(npart * sizeof *r_old);
	r_old = new double[npart][3];
	//v = (double (*)[3]) malloc(npart * sizeof *v);
	v = new double[npart][3];

	//Prepare array for measurements
	iv = 0; //Potential energy
	ik = 1; //Kinetic energy
	ie = 2; //Total energy
	it = 3; //Temperature
	n_props = 4; //Number of observables

	//Read initial configuration
	init_config(config, r);

	if (old.empty())
		init_vel();
	else
		init_config(old, r_old);
}

void MolDyn::info()
{
	cout << "Classic Lennard-Jones fluid" << endl;
	cout << "Molecular dynamics simulation in NVE ensemble" << endl;
	cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl;
	cout << "The program uses Lennard-Jones units" << endl;
	cout << "Number of particles = " << npart << endl;
	cout << "Density of particles = " << rho << endl;
	cout << "Volume of the simulation box = " << vol << endl;
	cout << "Edge of the simulation box = " << box << endl;
	cout << "The program integrates Newton equations with the Verlet method " << endl;
	cout << "Time step = " << delta << endl;
	cout << "Number of steps = " << nstep << endl << endl;
}

MolDyn::~MolDyn()
{
	delete r;
	delete r_old;
	delete v;
	delete gofr;
}

void MolDyn::rescale()
{
	double e_kin = 0.0;
	move();
	for (int i = 0; i < npart; i++) {
		for (int coord = 0; coord < 3; coord++)
			v[i][coord] = pbc(r[i][coord] - r_old[i][coord]) / delta;
		e_kin += v[i][0] * v[i][0] + v[i][1] * v[i][1] + v[i][2] * v[i][2]; // X
	}
	e_kin *= 0.5;
	double t = (2.0 / 3.0) * e_kin / (double) npart;

	double scale = sqrt(temp / t);
	for (int i = 0; i < npart; i++) {
		for (int coord = 0; coord < 3; coord++) {
			v[i][coord] *= scale;
			r_old[i][coord] = pbc(r[i][coord] - delta * v[i][coord]);
		}
	}
}

void MolDyn::move(void){ //Move particles with Verlet algorithm
	double r_new[3];
	double (*f)[3] = new double[npart][3];

	for (int i = 0; i < npart; i++) //Force acting on particle i
		for (int coord = 0; coord < 3; coord++)
			f[i][coord] = force(i, coord);

	for (int i = 0; i < npart; i++) {
		for (int coord = 0; coord < 3; coord++) {
			r_new[coord] = pbc(2.0 * r[i][coord] - r_old[i][coord] + f[i][coord] * delta * delta);
			v[i][coord] = pbc(r_new[coord] - r_old[i][coord]) / (2.0 * delta);
			r_old[i][coord] = r[i][coord];
			r[i][coord] = r_new[coord];
		}
	}

	delete f;
}

//Compute forces as -Grad_ip V(r)
double MolDyn::force(int ip, int idir) const
{
	double f=0.0;

	for (int i = 0; i < npart; i++) {
		if(i != ip) {
			double dvec[3];
			for (int coord = 0; coord < 3; coord++)
				dvec[coord] = pbc(r[ip][coord] - r[i][coord]);  // distance ip-i in pbc
			double dr = dvec[0] * dvec[0] + dvec[1] * dvec[1] + dvec[2] * dvec[2]; // X
			dr = sqrt(dr);

			if(dr < rcut)
				f += dvec[idir] * (48.0/pow(dr,14) - 24.0/pow(dr,8)); // -Grad_ip V(r)
		}
	}

	return f;
}

void MolDyn::measure()
{ //Properties measurement
	measured_epot = 0.0; //reset observables
	measured_ekin = 0.0;

	//cycle over pairs of particles
	for (int i = 0; i < npart - 1; i++) {
		for (int j = i + 1; j < npart; j++) {
			double dvec[3];
			// here I use old configurations [old = r(t)]
			 // to be compatible with EKin which uses v(t)
			 // => EPot should be computed with r(t)
			for (int coord = 0; coord < 3; coord++)
				dvec[coord] = pbc(r_old[i][coord] - r_old[j][coord]);
			double dr = dvec[0] * dvec[0] + dvec[1] * dvec[1] + dvec[2] * dvec[2]; // X
			dr = sqrt(dr);

			if (nbins != 0 and dr < box / 2.)
				gofr[(int) (2 * dr/box * nbins)] += 2;

			if(dr < rcut) {
				double vij = 4.0/pow(dr,12) - 4.0/pow(dr,6);
				//Potential energy
				measured_epot += vij;
			}
		}
	}
	measured_epot /= (double) npart; //Potential energy per particle

	//Kinetic energy
	for (int i = 0; i < npart; i++)
		measured_ekin += v[i][0] * v[i][0] + v[i][1] * v[i][1] + v[i][2] * v[i][2]; // X
	measured_ekin *= 0.5 / (double) npart; //Kinetic energy per particle

	measured_temp = (2.0 / 3.0) * measured_ekin; //Temperature
	measured_etot = measured_epot + measured_ekin; //Total energy per particle
}

void MolDyn::update_gofr()
{
	for (int i = 0; i < nbins; i++) {
		double bin_size = (box/2.0) / (double)nbins;
		double r = i * bin_size;
		double deltaV = 4. * M_PI/3. * (pow((r+bin_size), 3.) - pow(r, 3.));
		double norm = rho * npart * deltaV * n_throws;
		gofr[i] /= norm;
	}
}


void MolDyn::conf_final(string config, string old) const
{ //Write final configuration
	ofstream write_conf;

	write_conf.open(config);
	for (int i = 0; i < npart; i++)
		write_conf << r[i][0]/box << '\t' <<  r[i][1]/box << '\t' << r[i][2]/box << endl;
	write_conf.close();

	write_conf.open(old);
	for (int i = 0; i < npart; i++)
		write_conf << r_old[i][0]/box << '\t' <<  r_old[i][1]/box << '\t' << r_old[i][2]/box << endl;
	write_conf.close();
}

void MolDyn::conf_xyz(int nconf) const
{//Write configuration in .xyz format
	ofstream write_xyz;

	write_xyz.open("frames/config_" + to_string(nconf) + ".xyz");
	write_xyz << npart << endl;
	write_xyz << "This is only a comment!" << endl;
	for (int i=0; i<npart; ++i)
		write_xyz << "LJ  " << pbc(r[i][0]) << '\t' <<  pbc(r[i][1]) << '\t' << pbc(r[i][2]) << endl;
	write_xyz.close();
}

double MolDyn::pbc(double r) const
{  //Algorithm for periodic boundary conditions with side L=box
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
