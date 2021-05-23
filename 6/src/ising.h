/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __ISING__
#define __ISING__

//Random numbers
#include "random.h"

class Ising {
public:
	Random rnd;

	//parameters, observables
#define m_props 1000
	bool metropolis;
	int n_props,iu,ic,im,ix,ig;
	double nbins;
	double walker[m_props];

	// averages
	double blk_av[m_props],blk_norm;
	int accepted = 0, attempted = 0;
	double glob_av[m_props],glob_av2[m_props];
	double stima_u,stima_c,stima_m,stima_x,stima_g;
	double err_u,err_c,err_m,err_x,err_g;

	double internal_energy;
	double internal_energy2;
	double susceptivity;
	double magnetization;

	//configuration
#define m_spin 50
	double *s;

	// thermodynamical state
	int nspin;
	double beta,temp,J,h;

	// simulation
	int nstep, nblk;


	bool metropolis_ising(int k);
	void gibbs_ising(int k);

//public:
	//functions
	Ising(bool metropolis, int nspin, double temp, double J, double h);
	~Ising();
	void Reset(int);
	void Accumulate(void);
	void Averages(int);
	void move();
	void ConfFinal(void);
	void measure(void);
	double boltzmann(int);
	int Pbc(int);
	double Error(double,double,int);
};

#endif

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
