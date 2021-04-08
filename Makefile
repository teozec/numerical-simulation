.PHONY: all 1 2 3

all: 1 2 3

1 2 3:
	make -C $@

1.%:
	make -C 1 $@

2.%:
	make -C 2 $@

3.%:
	make -C 3 $@
