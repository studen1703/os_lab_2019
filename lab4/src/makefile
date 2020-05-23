CC=gcc
CFLAGS=-I.

all :  parallel_min_max t3 t5 t1

t1: 
	$(CC) /projects/os_lab_2019/lab3/src/parallel_min_max.c /projects/os_lab_2019/lab3/src/find_min_max.c utils.c -o task1 $(CFLAGS)
	./task1 --seed 1 --array_size 2000 --pnum 100 --timeout 1

t5 : utils.o sumlib.o 
	$(CC) -o task5 sumlib.o utils.o -lpthread parallel_sum.c $(CFLAGS)

sumlib.o: sumlib.h
	$(CC) -o sumlib.o -c sumlib.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h /projects/os_lab_2019/lab3/src/find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o /projects/os_lab_2019/lab3/src/parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h /projects/os_lab_2019/lab3/src/find_min_max.h
	$(CC) -o find_min_max.o -c /projects/os_lab_2019/lab3/src/find_min_max.c $(CFLAGS)

t3:
	$(CC) -o task3 process_memory.c 
	./task3

clean :
	rm utils.o find_min_max.o parallel_min_max t3
