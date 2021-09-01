rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic --std=c++11 -O3 -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=

.PHONY: all, clean
.SECONDARY:

all: 6.1
	make -C ${rng_dir}

6.%: obj/6.%.o obj/ising.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/6.$*.o obj/ising.o ${rng_dir}/obj/random.o -o $@

obj/%.o: src/%.cpp ${rng_dir}/random.h src/ising.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/$*.cpp -o $@

clean:
	rm -f obj/*
	rm -f 6.1
