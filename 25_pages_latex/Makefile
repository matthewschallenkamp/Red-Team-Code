all: pdf

test: quickcheck busted

clean:
	rm *.so

quickcheck: clibs
	lqc -c quickcheck

busted: clibs
	busted -p ".*" tests

pdf: test
	pdflatex -halt-on-error main.tex

clibs: libfenwick.so libprimes.so libgraph.so libfourier.so libkmp.so

libfourier.so: fourier/fft.cpp fourier/fft-interface.cpp
	g++ -shared -fPIC --std=c++11 -o $@ $^

libfenwick.so: fenwick/fenwick.c
	gcc -shared -fPIC -o libfenwick.so fenwick/fenwick.c

libprimes.so: primes/primes-interface.cpp primes/primes.cpp
	g++ -g -shared -fPIC --std=c++11 -o libprimes.so primes/primes.cpp primes/primes-interface.cpp

libgraph.so: graph/graph-interface.cpp graph/djikstra.cpp graph/graph.hpp
	g++ -shared -fPIC --std=c++11 -o libgraph.so graph/graph-interface.cpp graph/djikstra.cpp

libkmp.so: kmp/kmp-interface.cpp kmp/kmp.hpp
	g++ -shared -fPIC --std=c++11 -o libkmp.so kmp/kmp-interface.cpp
