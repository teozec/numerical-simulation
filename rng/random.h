/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef RANDOM_H
#define RANDOM_H

#include <string>

class Random {

private:
	int m1, m2, m3, m4, l1, l2, l3, l4, n1, n2, n3, n4;
	bool metropolis(double xn[], double x[], int ndim, double (*p)(double x[], int ndim));

protected:

public:
	// constructors
	Random();
	Random(std::string primes, std::string seed);
	// destructor
	~Random();
	// methods
	void set_random(int *, int, int);
	void save_seed(std::string filename);
	double rannyu(void);
	double rannyu(double min, double max);
	double gauss(double mean, double sigma);
	double exp(double lambda);
	double cauchy(double mean, double gamma);
	double accept_reject(double x1, double x2, double y_max, double (*p)(double x));
	bool metropolis_unif(double xn[], int ndim, double step, double (*p)(double x[], int ndim));
	bool metropolis_gauss(double xn[], int ndim, double sigma, double (*p)(double x[], int ndim));
};

#endif // RANDOM_H

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
