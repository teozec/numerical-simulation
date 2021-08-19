rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"' -O3
LDFLAGS:=

.PHONY: all, clean
.SECONDARY:

all: 9.1
	make -C ${rng_dir}

9.1: obj/9.1.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/9.1.o ${rng_dir}/obj/random.o -o $@

obj/9.1.o: src/9.1.cpp ${rng_dir}/random.h src/TSP.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/9.1.cpp -o $@

clean:
	rm -f obj/*
	rm -f 9.1
