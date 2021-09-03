/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <string>

class MolDyn {
	//parameters, observables
	const int m_props=4;
	int n_props;
	int iv, ik, it, ie;

	// averages
	double acc, att;

	//configuration
	const int m_part=108;
	double (*r)[3], (*r_old)[3];
	double (*v)[3];

	// thermodynamical state
	int npart;
	double energy, temp, vol, rho, box, rcut;

	// simulation
	int iprint, seed;
	double delta;

	//functions
	void init_input(std::string input);
	void init_config(std::string config, double r[][3]);
	void init_vel();
	double force(int, int) const;
	double pbc(double) const;

public:
	int nbins;
	int n_throws;
	double *gofr;
	double measured_epot, measured_ekin, measured_etot, measured_temp;
	int nstep;
	MolDyn(std::string input, std::string config, std::string old = std::string());
	~MolDyn();
	void info();
	void rescale();
	void move(void);
	void update_gofr();
	void conf_final(std::string config, std::string old) const;
	void conf_xyz(int) const;
	void measure(void);
};
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
