rng_dir:=../rng
seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-Wall -Wextra -pedantic -I ${rng_dir} -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=

.PHONY: all, rng, clean
.SECONDARY:

all: rng 5.1

rng:
	make -C ${rng_dir}

5.%: obj/5.%.o ${rng_dir}/obj/random.o
	${LD} ${LDFLAGS} obj/5.$*.o ${rng_dir}/obj/random.o -o $@

obj/%.o: src/%.cpp ${rng_dir}/random.h
	mkdir -p obj
	${CPP} ${CPPFLAGS} -c src/$*.cpp -o $@

clean:
	rm -f obj/*
	rm -f 5.1
