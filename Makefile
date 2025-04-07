n ?= 1000000
# path ?= sort/sort_numbers_parallel
# usage: make n=[whatever] path=[path to ur sorting executable]

serial: sort/sort_numbers
	python3 check.py $(n) sort/sort_numbers

parallel: sort/sort_numbers_parallel
	python3 check.py $(n) sort/sort_numbers_parallel

# sort/sort_numbers: sort/sort_numbers.ml
# 	ocamlc -o sort/sort_numbers sort/sort_numbers.ml

sort/sort_numbers: sort/sort_numbers.cpp
	g++ -std=c++11 -o sort/sort_numbers sort/sort_numbers.cpp

sort/sort_numbers_parallel: sort/sort_numbers_parallel.cpp
	g++ -std=c++11 -O3 -march=native -pthread -o sort/sort_numbers_parallel sort/sort_numbers_parallel.cpp

.PHONY: clean

clean:
	rm check/check_numbers **/*.class sort/sort_numbers **/*.cmo **/*.cmi generate/NumberGenerator.class
