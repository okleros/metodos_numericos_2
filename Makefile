all:
	cc -o main main.c -lm
run:
	cc -o main main.c -lm && ./main