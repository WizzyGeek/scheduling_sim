COMPILER=g++

all:
	$(COMPILER) src/main.cpp -o build/main
	build/main

run:
	build/main

clear:
	rm -rf bin
	rm -rf build
	mkdir bin && mkdir build