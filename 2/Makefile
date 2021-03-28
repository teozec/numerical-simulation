rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=

.PHONY: all, clean
.SECONDARY:

all: 2.1 2.2
	make -C ${rng_dir}

2.%: obj/2.%.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/2.$*.o ${rng_dir}/obj/random.o -o $@

obj/%.o: src/%.cpp ${rng_dir}/random.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/$*.cpp -o $@

clean:
	rm -f obj/*
	rm -f 2.1 2.2
