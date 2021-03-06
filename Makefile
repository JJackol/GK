SHELL := /bin/bash
TARGETS = $(patsubst %.cpp, %, $(wildcard main*.cpp))

all: $(TARGETS)

%: %.cpp
	g++ $< -o $@ -lGL -lglut -lm -lGLU

clean: 
	rm -f main*.o

purge: clean
	rm -f $(TARGETS)

