CFLAGS = -W -Wall -O2 -flto -save-temps
CC     = gcc

objects = function.o test.o

.PHONY: clean


target: function.o test.o
	$(CC) $(CFLAGS) $^ -o$@

$(objects): %.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o$@

clean:
	rm -f -- *.res *.o *.ii *.s *.out target
