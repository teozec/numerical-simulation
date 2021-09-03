rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=

.PHONY: all, clean
.SECONDARY:

all: 7.1.bin 7.2 7.3 7.4.nvt 7.4.nve
	make -C ${rng_dir}

7.1.bin: obj/7.1.o obj/monte_carlo.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/7.1.o obj/monte_carlo.o ${rng_dir}/obj/random.o -o $@

7.2: obj/7.2.o obj/monte_carlo.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/7.2.o obj/monte_carlo.o ${rng_dir}/obj/random.o -o $@

7.3: obj/7.3.o obj/mol_dyn.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/7.3.o obj/mol_dyn.o ${rng_dir}/obj/random.o -o $@

7.4.nvt: obj/7.4.nvt.o obj/monte_carlo.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/7.4.nvt.o obj/monte_carlo.o ${rng_dir}/obj/random.o -o $@

7.4.nve: obj/7.4.nve.o obj/mol_dyn.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/7.4.nve.o obj/mol_dyn.o ${rng_dir}/obj/random.o -o $@

obj/monte_carlo.o: src/monte_carlo.cpp ${rng_dir}/random.h src/monte_carlo.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/monte_carlo.cpp -o $@

obj/mol_dyn.o: src/mol_dyn.cpp ${rng_dir}/random.h src/mol_dyn.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/mol_dyn.cpp -o $@

obj/7.1.o: src/7.1.cpp ${rng_dir}/random.h src/monte_carlo.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/7.1.cpp -o $@

obj/7.2.o: src/7.2.cpp ${rng_dir}/random.h src/monte_carlo.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/7.2.cpp -o $@

obj/7.3.o: src/7.3.cpp ${rng_dir}/random.h src/mol_dyn.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/7.3.cpp -Wall -Wextra -o $@

obj/7.4.nvt.o: src/7.4.nvt.cpp ${rng_dir}/random.h src/monte_carlo.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/7.4.nvt.cpp -Wall -Wextra -o $@

obj/7.4.nve.o: src/7.4.nve.cpp ${rng_dir}/random.h src/mol_dyn.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/7.4.nve.cpp -Wall -Wextra -o $@

clean:
	rm -f obj/*
	rm -f 7.1.bin
	rm -f 7.2
	rm -f 7.3
	rm -f 7.4.nvt
	rm -f 7.4.nve
