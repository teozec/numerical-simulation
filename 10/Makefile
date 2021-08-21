rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"' -O3
LDFLAGS:=

.PHONY: all, clean
.SECONDARY:

all: 10.1
	make -C ${rng_dir}

10.1: obj/10.1.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/10.1.o ${rng_dir}/obj/random.o -o $@

obj/10.1.o: src/10.1.cpp ${rng_dir}/random.h src/TSP.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/10.1.cpp -o $@

clean:
	rm -f obj/*
	rm -f 10.1
