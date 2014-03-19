CC     = g++
CFLAGS = -W -Wall -O2 --std=c++11 $(LDLIBS)

targets = $(shell find . -path ./does-flto-do-function-inlining -prune -o -name '*.cpp' -print | sed -e 's_\(.*\)\.cpp$$_\1_g' )

.PHONY: all clean

all: $(targets)

pt:
	@echo $(targets)

#$(targets): %: %.cpp
#	$(CC) $(CFLAGS) -o$@ $^

perf-test-boost-vs-std: LDLIBS+=-lboost_system
perf-test-boost-vs-std: LDLIBS+=-lboost_system

clean: 
	rm -f $(targets)
