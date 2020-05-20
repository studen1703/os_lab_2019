CC=gcc
CFLAGS=-I.

all :  parallel_min_max task3
parallel_min_max : utils.o find_min_max.o /projects/os_lab_2019/lab3/src/utils.h /projects/os_lab_2019/lab3/src/find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o /projects/os_lab_2019/lab3/src/parallel_min_max.c $(CFLAGS)

utils.o : /projects/os_lab_2019/lab3/src/utils.h
	$(CC) -o utils.o -c /projects/os_lab_2019/lab3/src/utils.c $(CFLAGS)

find_min_max.o : /projects/os_lab_2019/lab3/src/utils.h /projects/os_lab_2019/lab3/src/find_min_max.h
	$(CC) -o find_min_max.o -c /projects/os_lab_2019/lab3/src/find_min_max.c $(CFLAGS)

task3:
	$(CC) -o t3 process_memory.c 
	./t3

clean :
	rm utils.o find_min_max.o parallel_min_max t3
