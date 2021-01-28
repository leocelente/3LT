

all: main.out\
	child.out\
	parent.out\

%.out: %.c 
	$(CC) comm.c $*.c -g -o $*.out 

run:
	./main.out

clean:
	rm ./*.out
