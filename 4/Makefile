rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -O3 -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=

.PHONY: all clean 4.1 4.2 4.3
.SECONDARY:

all: 4.1 4.2 4.3

4.1: 4.1.bin

4.2: 4.2.bin

4.3: 4.3.bin

4.%.bin: obj/4.%.o obj/mol_dyn.o
	${LD} ${LDFLAGS} obj/4.$*.o obj/mol_dyn.o -o $@

obj/%.o: src/%.cpp src/mol_dyn.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/$*.cpp -o $@

clean:
	rm -f obj/*
	rm -f 4.1.bin 4.2.bin 4.3.bin
