all:
	cc -o main main.c -lm -O3
run:
	cc -o main main.c -lm -O3 && ./main