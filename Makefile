seed_dir:=../seed

CPP:=g++
LD:=g++
CPPFLAGS:=-std=c++11 -O3 -I random -D 'SEED_DIR="${seed_dir}"'
LDFLAGS:=


.PHONY: all 1 2 3 4 5 6 7 8 9 10 clean 1-clean 2-clean 3-clean 4-clean 5-clean 6-clean 7-clean 8-clean 9-clean 10-clean

all: 1 2 3 4 5 6 7 8 9 10
clean: 1-clean 2-clean 3-clean 4-clean 5-clean 6-clean 7-clean 8-clean 9-clean 10-clean

# Random number generator
random/obj/random.o: random/random.cpp random/random.h
	mkdir -p random/obj
	${CPP} ${CPPFLAGS} -c random/random.cpp -o random/obj/random.o

# NVE molecular dynamics
nve/obj/nve.o: nve/nve.cpp nve/nve.h
	mkdir -p nve/obj
	${CPP} ${CPPFLAGS} -c nve/nve.cpp -o nve/obj/nve.o

# NVT molecular dynamics
nvt/obj/nvt.o: nvt/nvt.cpp nvt/nvt.h
	mkdir -p nvt/obj
	${CPP} ${CPPFLAGS} -c nvt/nvt.cpp -o nvt/obj/nvt.o

# Ising model
ising/obj/ising.o: ising/ising.cpp ising/ising.h
	mkdir -p ising/obj
	${CPP} ${CPPFLAGS} -c ising/ising.cpp -o ising/obj/ising.o

# Exercise 1
1: 1/1.1 1/1.2 1/1.3

1/1.1: 1/obj/1.1.o random/obj/random.o
	${LD} ${LDFLAGS} 1/obj/1.1.o random/obj/random.o -o $@
1/obj/1.1.o: 1/src/1.1.cpp random/random.h
	mkdir -p 1/obj
	${CPP} ${CPPFLAGS} -c 1/src/1.1.cpp -o $@

1/1.2: 1/obj/1.2.o random/obj/random.o
	${LD} ${LDFLAGS} 1/obj/1.2.o random/obj/random.o -o $@
1/obj/1.2.o: 1/src/1.2.cpp random/random.h
	mkdir -p 1/obj
	${CPP} ${CPPFLAGS} -c 1/src/1.2.cpp -o $@

1/1.3: 1/obj/1.3.o random/obj/random.o
	${LD} ${LDFLAGS} 1/obj/1.3.o random/obj/random.o -o $@
1/obj/1.3.o: 1/src/1.3.cpp random/random.h
	mkdir -p 1/obj
	${CPP} ${CPPFLAGS} -c 1/src/1.3.cpp -o $@

1-clean:
	rm -f 1/obj/*
	rm -f 1/1.1 1/1.2 1/1.3

# Exercise 2
2: 2/2.1 2/2.2

2/2.1: 2/obj/2.1.o random/obj/random.o
	${LD} ${LDFLAGS} 2/obj/2.1.o random/obj/random.o -o $@
2/obj/2.1.o: 2/src/2.1.cpp random/random.h
	mkdir -p 2/obj
	${CPP} ${CPPFLAGS} -c 2/src/2.1.cpp -o $@

2/2.2: 2/obj/2.2.o random/obj/random.o
	${LD} ${LDFLAGS} 2/obj/2.2.o random/obj/random.o -o $@
2/obj/2.2.o: 2/src/2.2.cpp random/random.h
	mkdir -p 2/obj
	${CPP} ${CPPFLAGS} -c 2/src/2.2.cpp -o $@

2-clean:
	rm -f 2/obj/*
	rm -f 2/2.1 2/2.2

# Exercise 3
3: 3/3.1

3/3.1: 3/obj/3.1.o random/obj/random.o
	${LD} ${LDFLAGS} 3/obj/3.1.o random/obj/random.o -o $@
3/obj/3.1.o: 3/src/3.1.cpp random/random.h
	mkdir -p 3/obj
	${CPP} ${CPPFLAGS} -c 3/src/3.1.cpp -o $@

3-clean:
	rm -f 3/obj/*
	rm -f 3/3.1

# Exercise 4
4: 4/4.1.bin 4/4.2.bin 4/4.3.bin

4/4.1.bin: 4/obj/4.1.o random/obj/random.o nve/obj/nve.o
	${LD} ${LDFLAGS} 4/obj/4.1.o random/obj/random.o nve/obj/nve.o -o $@
4/obj/4.1.o: 4/src/4.1.cpp random/random.h nve/nve.h
	mkdir -p 4/obj
	${CPP} ${CPPFLAGS} -I nve -c 4/src/4.1.cpp -o $@

4/4.2.bin: 4/obj/4.2.o random/obj/random.o nve/obj/nve.o
	${LD} ${LDFLAGS} 4/obj/4.2.o random/obj/random.o nve/obj/nve.o -o $@
4/obj/4.2.o: 4/src/4.2.cpp random/random.h nve/nve.h
	mkdir -p 4/obj
	${CPP} ${CPPFLAGS} -c -I nve 4/src/4.2.cpp -o $@

4/4.3.bin: 4/obj/4.3.o random/obj/random.o nve/obj/nve.o
	${LD} ${LDFLAGS} 4/obj/4.3.o random/obj/random.o nve/obj/nve.o -o $@
4/obj/4.3.o: 4/src/4.3.cpp random/random.h nve/nve.h
	mkdir -p 4/obj
	${CPP} ${CPPFLAGS} -c -I nve 4/src/4.3.cpp -o $@

4-clean:
	rm -f 4/obj/*
	rm -f 4/*.bin

# Exercise 5
5: 5/5.1

5/5.1: 5/obj/5.1.o random/obj/random.o
	${LD} ${LDFLAGS} 5/obj/5.1.o random/obj/random.o -o $@
5/obj/5.1.o: 5/src/5.1.cpp random/random.h
	mkdir -p 5/obj
	${CPP} ${CPPFLAGS} -c 5/src/5.1.cpp -o $@

5-clean:
	rm -f 5/obj/*
	rm -f 5/5.1

# Exercise 6
6: 6/6.1

6/6.1: 6/obj/6.1.o random/obj/random.o ising/obj/ising.o
	${LD} ${LDFLAGS} 6/obj/6.1.o random/obj/random.o ising/obj/ising.o -o $@
6/obj/6.1.o: 6/src/6.1.cpp ising/ising.h
	mkdir -p 6/obj
	${CPP} ${CPPFLAGS} -c -I ising 6/src/6.1.cpp -o $@

6-clean:
	rm -f 6/obj/*
	rm -f 6/6.1

# Exercise 7
7: 7/7.1.bin 7/7.2 7/7.3 7/7.4.nve 7/7.4.nvt

7/7.1.bin: 7/obj/7.1.o random/obj/random.o nvt/obj/nvt.o
	${LD} ${LDFLAGS} 7/obj/7.1.o random/obj/random.o nvt/obj/nvt.o -o $@
7/obj/7.1.o: 7/src/7.1.cpp random/random.h nvt/nvt.h
	mkdir -p 7/obj
	${CPP} ${CPPFLAGS} -I nvt -c 7/src/7.1.cpp -o $@

7/7.2: 7/obj/7.2.o random/obj/random.o nvt/obj/nvt.o
	${LD} ${LDFLAGS} 7/obj/7.2.o random/obj/random.o nvt/obj/nvt.o -o $@
7/obj/7.2.o: 7/src/7.2.cpp random/random.h nvt/nvt.h
	mkdir -p 7/obj
	${CPP} ${CPPFLAGS} -I nvt -c 7/src/7.2.cpp -o $@

7/7.3: 7/obj/7.3.o random/obj/random.o nve/obj/nve.o
	${LD} ${LDFLAGS} 7/obj/7.3.o random/obj/random.o nve/obj/nve.o -o $@
7/obj/7.3.o: 7/src/7.3.cpp random/random.h nve/nve.h
	mkdir -p 7/obj
	${CPP} ${CPPFLAGS} -I nve -c 7/src/7.3.cpp -o $@

7/7.4.nvt: 7/obj/7.4.nvt.o random/obj/random.o nvt/obj/nvt.o
	${LD} ${LDFLAGS} 7/obj/7.4.nvt.o random/obj/random.o nvt/obj/nvt.o -o $@
7/obj/7.4.nvt.o: 7/src/7.4.nvt.cpp random/random.h nvt/nvt.h
	mkdir -p 7/obj
	${CPP} ${CPPFLAGS} -I nvt -c 7/src/7.4.nvt.cpp -o $@

7/7.4.nve: 7/obj/7.4.nve.o random/obj/random.o nve/obj/nve.o
	${LD} ${LDFLAGS} 7/obj/7.4.nve.o random/obj/random.o nve/obj/nve.o -o $@
7/obj/7.4.nve.o: 7/src/7.4.nve.cpp random/random.h nve/nve.h
	mkdir -p 7/obj
	${CPP} ${CPPFLAGS} -I nve -c 7/src/7.4.nve.cpp -o $@

7-clean:
	rm -f 7/obj/*
	rm -f 7/7.1.bin
	rm -f 7/7.2
	rm -f 7/7.3
	rm -f 7/7.4.nvt
	rm -f 7/7.4.nve

# Exercise 8
8: 8/8.1 8/8.2.bin 8/8.3

8/8.1: 8/obj/8.1.o random/obj/random.o
	${LD} ${LDFLAGS} 8/obj/8.1.o random/obj/random.o -o $@
8/obj/8.1.o: 8/src/8.1.cpp random/random.h
	mkdir -p 8/obj
	${CPP} ${CPPFLAGS} -c 8/src/8.1.cpp -o $@

8/8.2.bin: 8/obj/8.2.o random/obj/random.o
	${LD} ${LDFLAGS} 8/obj/8.2.o random/obj/random.o -o $@
8/obj/8.2.o: 8/src/8.2.cpp random/random.h
	mkdir -p 8/obj
	${CPP} ${CPPFLAGS} -c 8/src/8.2.cpp -o $@

8/8.3: 8/obj/8.3.o random/obj/random.o
	${LD} ${LDFLAGS} `root-config --libs` 8/obj/8.3.o random/obj/random.o -o $@
8/obj/8.3.o: 8/src/8.3.cpp random/random.h
	mkdir -p 8/obj
	${CPP} ${CPPFLAGS} `root-config --cflags` -c 8/src/8.3.cpp -o $@


8-clean:
	rm -f 8/obj/*
	rm -f 8/8.1
	rm -f 8/8.2.bin
	rm -f 8/8.3

# Exercise 9
9: 9/9.1

9/9.1: 9/obj/9.1.o random/obj/random.o
	${LD} ${LDFLAGS} 9/obj/9.1.o random/obj/random.o -o $@
9/obj/9.1.o: 9/src/9.1.cpp random/random.h
	mkdir -p 9/obj
	${CPP} ${CPPFLAGS} -c 9/src/9.1.cpp -o $@

9-clean:
	rm -f 9/obj/*
	rm -f 9/9.1

# Exercise 10
10: 10/10.1 10/10.2.bin

10/10.1: 10/obj/10.1.o random/obj/random.o
	${LD} ${LDFLAGS} 10/obj/10.1.o random/obj/random.o -o $@
10/obj/10.1.o: 10/src/10.1.cpp random/random.h
	mkdir -p 10/obj
	${CPP} ${CPPFLAGS} -c 10/src/10.1.cpp -o $@

10/10.2.bin: 10/obj/10.2.o random/obj/random.o
	mpicxx ${LDFLAGS} 10/obj/10.2.o random/obj/random.o -o $@
10/obj/10.2.o: 10/src/10.2.cpp random/random.h
	mkdir -p 10/obj
	mpicxx ${CPPFLAGS} -c 10/src/10.2.cpp -o $@

10-clean:
	rm -f 10/obj/*
	rm -f 10/10.1
	rm -f 10/10.2.bin
