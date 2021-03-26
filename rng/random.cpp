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
#include <cmath>
#include <cstdlib>
#include <string>
#include "random.h"

using namespace std;

Random :: Random(){}

Random :: ~Random(){}

Random::Random(string primes_file, string seed_file) {
	int seed[4];
	int p1, p2;
	ifstream primes(primes_file);
	if (primes.is_open()){
		primes >> p1 >> p2 ;
	} else {
		cerr << "PROBLEM: Unable to open " << primes_file << endl;
		exit(1);
	}
	primes.close();

	ifstream input(seed_file);
	string property;
	if (input.is_open()){
		while ( !input.eof() ){
			input >> property;
			if (property == "RANDOMSEED"){
				input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
				set_random(seed, p1, p2);
			}
		}
		input.close();
	} else {
		cerr << "PROBLEM: Unable to open " << seed_file << endl;
		exit(1);
	}
}

void Random::save_seed(string filename){
	ofstream write_seed;
	write_seed.open(filename);
	if (write_seed.is_open()){
		write_seed << l1 << " " << l2 << " " << l3 << " " << l4 << endl;;
	} else cerr << "PROBLEM: Unable to open " << filename << endl;
	write_seed.close();
	return;
}

double Random :: gauss(double mean, double sigma) {
	double s=rannyu();
	double t=rannyu();
	double x=sqrt(-2.*log(1.-s))*cos(2.*M_PI*t);
	return mean + x * sigma;
}

double Random :: rannyu(double min, double max){
	return min+(max-min)*rannyu();
}

double Random :: rannyu(void){
	const double twom12=0.000244140625;
	int i1,i2,i3,i4;
	double r;

	i1 = l1*m4 + l2*m3 + l3*m2 + l4*m1 + n1;
	i2 = l2*m4 + l3*m3 + l4*m2 + n2;
	i3 = l3*m4 + l4*m3 + n3;
	i4 = l4*m4 + n4;
	l4 = i4%4096;
	i3 = i3 + i4/4096;
	l3 = i3%4096;
	i2 = i2 + i3/4096;
	l2 = i2%4096;
	l1 = (i1 + i2/4096)%4096;
	r=twom12*(l1+twom12*(l2+twom12*(l3+twom12*(l4))));

	return r;
}

void Random :: set_random(int * s, int p1, int p2){
	m1 = 502;
	m2 = 1521;
	m3 = 4071;
	m4 = 2107;
	l1 = s[0];
	l2 = s[1];
	l3 = s[2];
	l4 = s[3];
	n1 = 0;
	n2 = 0;
	n3 = p1;
	n4 = p2;

	return;
}

double Random::exp(double lambda) {
	return -log(1-rannyu()) / lambda;
}

double Random::cauchy(double mean, double gamma) {
	return mean + gamma * tan(M_PI * (rannyu() - 0.5));
}

double Random::accept_reject(double x1, double x2, double y_max, double (*p)(double x)) {
	double x;
	double y;
	do {
		x = rannyu(x1, x2);
		y = rannyu(0., y_max);
	} while (p(x) < y);
	return x;
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
