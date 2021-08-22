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
#include <functional>

class Random {

private:
	int m1, m2, m3, m4, l1, l2, l3, l4, n1, n2, n3, n4;
	bool metropolis(double xn[], double x[], int ndim, std::function<double(double [], int)> p);

protected:

public:
	// constructors
	Random(std::string primes, std::string seed);
	// methods
	void set_random(int *, int, int);
	void save_seed(std::string filename);
	double rannyu(void);
	double rannyu(double min, double max);
	double gauss(double mean, double sigma);
	double exp(double lambda);
	double cauchy(double mean, double gamma);
	double accept_reject(double x1, double x2, double y_max, std::function<double(double)> p);

	template <class T> bool metropolis(T &xn, T &x, std::function<double(T&)> p) {
		bool accept = false;
		double alpha = p(x) / p(xn);

		// Accept x with probability alpha (otherwise keep old x)
		if (alpha > 1. or rannyu() <= alpha) {
			xn = x;
			accept = true;
		}

		return accept;
	}

	bool metropolis_unif(double xn[], int ndim, double step, std::function<double(double [], int)> p);
	bool metropolis_gauss(double xn[], int ndim, double sigma, std::function<double(double [], int)> p);

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
