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
#include "ising.h"

using namespace std;

Ising::Ising(bool metropolis, int nspin, double temp, double J, double h):
	rnd(SEED_DIR "/Primes", SEED_DIR "/seed.in"),
	metropolis{metropolis}, nspin{nspin}, temp{temp}, J{J}, h{h}
{
	beta = 1.0/temp;
	s = new double[nspin];

	//initial configuration
	for (int i = 0; i < nspin; i++)
		s[i] = (rnd.rannyu() >= 0.5) ? 1. : -1.;
}

Ising::~Ising()
{
	delete s;
}

bool Ising::metropolis_ising(int k)
{
	bool accept = false;
	double alpha = boltzmann(k);

	if (rnd.rannyu() <= alpha) {
		s[k] = -s[k];
		accept = true;
	}
	
	return accept;
}

void Ising::gibbs_ising(int k)
{
	s[k] = 1;
	s[k] = rnd.rannyu() > (1. / (1. + boltzmann(k))) ? -1 : 1;
}


void Ising::move()
{
	for (int i = 0; i < nspin; i++) {
		int k = rnd.rannyu() * nspin;
		if (metropolis) {
			attempted++;
			accepted += metropolis_ising(k);
		} else {
			attempted++;
			accepted++;
			gibbs_ising(k);
    		}
  	}
}

double Ising::boltzmann(int k)
{
	double ene = -2. * s[k] * (J * (s[Pbc(k-1)] + s[Pbc(k+1)]) + h);
	return exp(beta * ene); // ? - sign
}

void Ising::measure()
{
	internal_energy = 0.;
	magnetization = 0.;

	//cycle over spins
	for (int i = 0; i < nspin; i++) {
		internal_energy += -J * s[i] * s[Pbc(i+1)] - 0.5 * h * (s[i] + s[Pbc(i+1)]);
		magnetization += s[i];
	}
	internal_energy /= nspin;
	internal_energy2 = internal_energy * internal_energy * nspin;
	magnetization /= nspin;
	susceptivity = beta * magnetization * magnetization * nspin;
}


//void Ising::Reset(int iblk) //Reset block averages
//{
	//if(iblk == 1)
	//{
		//for(int i=0; i<n_props; ++i) {
			//glob_av[i] = 0;
			//glob_av2[i] = 0;
		//}
	//}

	//for(int i=0; i<n_props; ++i) {
		//blk_av[i] = 0;
	//}
	//blk_norm = 0;
	//attempted = 0;
	//accepted = 0;
//}


//void Ising::Accumulate(void) //Update block averages
//{
	//for(int i=0; i<n_props; ++i) {
		//blk_av[i] = blk_av[i] + walker[i];
	//}
	//blk_norm = blk_norm + 1.0;
//}


//void Ising::Averages(int iblk) //Print results for current block
//{
	//ofstream Ene, Heat, Mag, Chi;
	//const int wd=12;

	//cout << "Block number " << iblk << endl;
	//cout << "Acceptance rate " << (double) accepted / attempted << endl << endl;
 
	//Ene.open("output.ene.0",ios::app);
	//stima_u = blk_av[iu]/blk_norm/(double)nspin; //Energy
	//glob_av[iu]  += stima_u;
	//glob_av2[iu] += stima_u*stima_u;
	//err_u=Error(glob_av[iu],glob_av2[iu],iblk);
	//Ene << setw(wd) << iblk <<  setw(wd) << stima_u << setw(wd) << glob_av[iu]/(double)iblk << setw(wd) << err_u << endl;
	//Ene.close();

// INCLUDE YOUR CODE HERE

	//cout << "----------------------------" << endl << endl;
//}


void Ising::ConfFinal(void)
{
	ofstream WriteConf;

	cout << "Print final configuration to file config.final " << endl << endl;
	WriteConf.open("config.final");
	for (int i=0; i<nspin; ++i) {
		WriteConf << s[i] << endl;
	}
	WriteConf.close();

	rnd.save_seed(SEED_DIR "/seed.out");
}

int Ising::Pbc(int i)  //Algorithm for periodic boundary conditions
{
	if (i >= nspin)
		i -=  nspin;
	else if (i < 0)
		i += nspin;
	return i;
}

double Ising::Error(double sum, double sum2, int iblk)
{
	if(iblk==1) return 0.0;
	else return sqrt((sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)(iblk-1));
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
