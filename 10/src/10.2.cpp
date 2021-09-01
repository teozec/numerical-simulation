#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <numeric>
#include "TSP.h"
#include "mpi.h"

// Exercise 10.2
// TSP using MPI

using namespace std;

double cost_accumulate(vector<Individual> individuals, int n_individuals)
{
	// Use the C++ accumulate function to sum the costs of the individuals in the vector, then divide by their number
	return accumulate(individuals.begin(), individuals.begin() + n_individuals, individuals[0].cost(),
		[](const double x, const Individual &y) { return x + y.cost(); }) / n_individuals;
}

// TSP algorithm
void TSP(Random rnd, const int n_cities, const int n_individuals, const int n_steps,
	const int n_generations, vector<City> city, string s, int rank, double pm, double pc, double p)
{

	vector<Individual> individuals;
	// Populate a vector of individuals (each individual is a random path to connect all cities)
	for (int i = 0; i < n_individuals; i++)
		individuals.push_back(Individual{city, rnd});

	// Create a population object, containing the vector of individuals
	Population population{individuals, rnd, pm, pc, p};
	ofstream out{"data/10.2-" + s + "-" + to_string(rank) + "-out.dat"};
	if (!out.is_open()) {
		MPI_Finalize();
		cerr << "Could not open data/10.2-" + s + "-" + to_string(rank) + "-out.dat" << endl;
		exit(1);
	}

	// Write to the output file the cost of the best individual and the mean cost of the best half of the population
	out << population.individuals[0].cost() << '\t' << cost_accumulate(population.individuals, n_individuals / 2) << endl;
	for (int i = 0; i < n_steps; i++) {
		// Evolve the population for n_generations
		for (int j = 0; j < n_generations; j++) {
			population.new_generation();
			out << population.individuals[0].cost() << '\t' << cost_accumulate(population.individuals, n_individuals / 2) << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
		int cont;
		// Thread 0 selects a random continent to exchange the best with.
		// The other two threads will exchange their bests.
		if (rank == 0)
			cont = (int) rnd.rannyu(1., 4.);
		MPI_Bcast(&cont, 1, MPI_INT, 0, MPI_COMM_WORLD);
		vector<City> send{population.individuals[0].cities};
		vector<City> recv{send.size()};
		size_t size = send.size() * sizeof(decltype(send)::value_type);

		// The process with lower rank does send-recv, the one with higher rank does recv-send.
		if (rank == 0) {	// 0: send-recv with cont
			MPI_Sendrecv(send.data(), size, MPI_BYTE, cont, 0, recv.data(), size, MPI_BYTE, cont, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);

		} else if (rank == cont) {	// cont: recv-send with 0
			MPI_Recv(recv.data(), size, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
			MPI_Send(send.data(), size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
		} else if (rank == 1) {	// 1, but not cont: send-recv with 2 or 3
			if (cont == 2)
				MPI_Sendrecv(send.data(), size, MPI_BYTE, 3, 0, recv.data(), size, MPI_BYTE, 3, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
			else
				MPI_Sendrecv(send.data(), size, MPI_BYTE, 2, 0, recv.data(), size, MPI_BYTE, 2, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
		} else if (rank == 2) {	// 2, but not cont: send-recv with 3, or recv-send with 1
			if (cont == 1) {
				MPI_Sendrecv(send.data(), size, MPI_BYTE, 3, 0, recv.data(), size, MPI_BYTE, 3, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
			} else {
				MPI_Recv(recv.data(), size, MPI_BYTE, 1, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
				MPI_Send(send.data(), size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
			}
		} else {	// 3, but not cont: recv-send with 1 or 2
			if (cont == 1) {
				MPI_Recv(recv.data(), size, MPI_BYTE, 2, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
				MPI_Send(send.data(), size, MPI_BYTE, 2, 0, MPI_COMM_WORLD);
			} else {
				MPI_Recv(recv.data(), size, MPI_BYTE, 1, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
				MPI_Send(send.data(), size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		population.individuals[0].cities = recv;
		population.sort();
	}

	// Write the best path to the output file
	ofstream best{"data/10.2-" + s + "-" + to_string(rank) + "-best.dat"};
	if (!best.is_open()) {
		cerr << "Could not open data/10.2-" + s + "-" + to_string(rank) + "-best.dat" << endl;
		exit(1);
	}
	for (int i = 0; i < n_cities; i++)
		best << population.individuals[0].cities[i].label << endl;
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int size;
	// Get number of threads
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size != 4) {
		MPI_Finalize();
		cerr << "Error: must have 4 processes!" << endl;
		return 1;
	}

	int rank;
	// Get rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	Random rnd{SEED_DIR "/Primes", "seed/seed." + to_string(rank)};
	const int n_cities = 32;
	const int n_individuals = 5000;
	const int n_steps = 20;
	const int n_generations = 15;

	vector<City> circ{n_cities};
	MPI_Barrier(MPI_COMM_WORLD);
	// Process 0 generates the cities placed on a circle
	if (rank == 0) {
		ofstream out_cities{"data/10.2-circ-cities.dat"};
		if (!out_cities.is_open()) {
			MPI_Finalize();
			cerr << "Could not open data/10.2-circ-cities.dat" << endl;
			exit(1);
		}
		for (int i = 0; i < n_cities; i++) {
			double theta = rnd.rannyu(0, 2*M_PI);
			circ[i] = City{cos(theta), sin(theta), i};
			out_cities << i << '\t' << cos(theta) << '\t' << sin(theta) << endl;
		}
	}
	// Broadcast the cities to all threads
	MPI_Bcast(circ.data(), circ.size() * sizeof(decltype(circ)::value_type), MPI_BYTE, 0, MPI_COMM_WORLD);
	// Call TSP algorithm
	TSP(rnd, n_cities, n_individuals, n_steps, n_generations, circ, "circ", rank, 0.15, 0.7, 2.);

	vector<City> square{n_cities};
	MPI_Barrier(MPI_COMM_WORLD);
	// Process 0 generates the cities square
	if (rank == 0) {
		ofstream out_cities{"data/10.2-square-cities.dat"};
		if (!out_cities.is_open()) {
			MPI_Finalize();
			cerr << "Could not open data/10.2-square-cities.dat" << endl;
			exit(1);
		}
		for (int i = 0; i < n_cities; i++) {
			double x = rnd.rannyu();
			double y = rnd.rannyu();
			square[i] = City{x, y, i};
			out_cities << i << '\t' << x << '\t' << y << endl;
		}
	}
	MPI_Bcast(square.data(), square.size() * sizeof(decltype(square)::value_type), MPI_BYTE, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	TSP(rnd, n_cities, n_individuals, n_steps, n_generations, square, "square", rank, 0.1, 0.7, 2.);

	MPI_Finalize();

	return 0;
}
