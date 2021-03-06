# numerical-simulation

A collection of solved exercises for the _Numerical simulation laboratory_ course ([EN](https://www.unimi.it/en/education/degree-programme-courses/2021/numerical-simulation-laboratory-0), [IT](https://www.unimi.it/it/corsi/insegnamenti-dei-corsi-di-laurea/2021/laboratorio-di-simulazione-numerica-0)), held by [Prof. Davide Emilio Galli](http://materia.fisica.unimi.it/~dgalli/) in 2021.


## Building

To build all the exercises, please run:
```bash
make all
```
If you only wish to build the exercises for lesson 1, run:
```bash
make 1
```


## Executing

To run exercise, for example, 1.2, make sure that the directory `1/data/` exists, then execute the following:
```bash
cd 1/
./1.2
```


## Visualizing the results

Finally, to check the results of the simulations and perform the final data analysis, you can run
```bash
jupyter lab
```
and navigate to the desired notebook situated inside the exercise directory.

If needed, the C++ code is situated in the `src/` subdirectory in each exercise. Sometimes, the executable is a shell script that calls a compiled binary, such as `4.3` which calls `4.3.bin`.
