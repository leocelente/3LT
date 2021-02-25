

all: src/main.out\
	src/peripheral/slave.out\
	src/microcontroller/master.out\

%.out: %.c 
	$(CC) -std=gnu11 src/protocol-channel/comm.c $*.c -g -o $*.out 

run:
	./main.out

clean:
	rm ./*.out
