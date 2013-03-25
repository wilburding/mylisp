.PHONY: all mylisp test clean

all: mylisp test

mylisp:
	$(MAKE) -C mylisp

test:
	$(MAKE) -C test

clean:
	$(MAKE) -C mylisp clean
	$(MAKE) -C test clean
