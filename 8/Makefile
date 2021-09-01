rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -O3 --std=c++11 -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=

.PHONY: all, rng, clean
.SECONDARY:

all: rng 8.1 8.2.bin 8.3

rng:
	make -C ${rng_dir}

8.1: obj/8.1.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/8.1.o ${rng_dir}/obj/random.o -o $@

8.2.bin: obj/8.2.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/8.2.o ${rng_dir}/obj/random.o -o $@

8.3: obj/8.3.o
	${LD} ${LDFLAGS} `root-config --libs` obj/8.3.o -o $@

obj/8.3.o: src/8.3.cpp src/functions.h src/constants.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} `root-config --cflags` -c src/8.3.cpp -o $@

obj/%.o: src/%.cpp ${rng_dir}/random.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/$*.cpp -o $@

clean:
	-rm -f obj/*
	-rm -f 8.1
	-rm -f 8.2.bin
	-rm -f 8.3
