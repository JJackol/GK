SHELL := /bin/bash
TARGETS = $(patsubst %.cpp, %, $(wildcard main*.cpp))

all: $(TARGETS)

%: %.cpp
	g++ $< -o $@ -lGL -lglut -lm

clean: 
	rm -f main*.cpp

purge: clean
	rm -f $(TARGETS)

