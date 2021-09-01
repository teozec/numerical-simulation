rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -std=c++11 -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=

.PHONY: all, clean
.SECONDARY:

all: 1.1 1.2 1.3
	make -C ${rng_dir}

1.%: obj/1.%.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/1.$*.o ${rng_dir}/obj/random.o -o $@

obj/%.o: src/%.cpp ${rng_dir}/random.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/$*.cpp -o $@

clean:
	-rm -f obj/*
	-rm -f 1.1 1.2 1.3
