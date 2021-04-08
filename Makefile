.PHONY: all 1 2

all: 1 2

1 2:
	make -C $@

1.%:
	make -C 1 $@

2.%:
	make -C 2 $@
