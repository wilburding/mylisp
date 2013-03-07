.PHONY: mylisp test clean

mylisp:
	$(MAKE) -C mylisp

test:
	$(MAKE) -C test

clean:
	$(MAKE) -C mylisp clean
