/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef NVT_H_DEFINED
#define NVT_H_DEFINED

#include <string>
//Random numbers
#include "random.h"

class NVT {
public:
	int seed[4];
	Random rnd;

//parameters, observables
#define m_props 1000
	int n_throws;
	int n_props, iv, iw, igofr;
	double vtail,ptail,bin_size;
	int nbins;
	double sd;
	double walker[m_props];

// averages
	double blk_av[m_props],blk_norm,accepted = 0,attempted = 0;
	double glob_av[m_props],glob_av2[m_props];
	double stima_pot,stima_pres,err_pot,err_press,err_gdir;
	double energy, pressure;

//configuration
#define m_part 108
	double x[m_part],y[m_part],z[m_part];

// thermodynamical state
	int npart;
	double beta,temp,vol,rho,box,rcut;

// simulation
	int nstep, nblk;
	double delta;

//pigreco
	const double pi=3.1415927;

//functions
	NVT(std::string input_file, std::string config_file);
	void move(void);
	void update_gofr();
	void ConfFinal(void);
	void ConfXYZ(int);
	void measure(void);
	void add_tails(double &energy, double &pressure);
	double boltzmann(double, double, double, int);
	double Pbc(double);
};

#endif // NVT_H_DEFINED

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
