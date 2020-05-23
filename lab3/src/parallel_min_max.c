#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

#include <unistd.h>   //Для alarm
#include <signal.h>   //Для signal

int child_pid;
void MyAlarm (int t)
{     
    printf("child_pid = %d, killed\n", child_pid);
    kill(child_pid, SIGKILL);
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = -1;
  signal(SIGALRM, MyAlarm);

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"timeout", required_argument, 0, 't'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "t", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if (seed <= 0) {
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (pnum <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 't':
        timeout = atoi(optarg);
        printf("timeout = %d\n", timeout);
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
    
  int i;

  int part = (float)array_size / pnum;

  int *pipe_array;
  int *file_pipe; 
 
  pipe_array = (int *)malloc(sizeof(int) * 2 * pnum);


  for ( i = 0; i < pnum; i++) {
    file_pipe = pipe_array + (i * 2); //для каждого потока своя пара дескрипторов
		if(pipe(file_pipe) == -1)
			exit(EXIT_FAILURE);
	
    alarm(timeout);
    child_pid = fork();
    
    if (child_pid >= 0) {
      // successful fork

      printf("cp = %d\n", child_pid);
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        sleep(10);
        printf("Succesfull fork: %d %d\n ", i, getpid());
        int arr_start = part * (float)i;
        int arr_end = arr_start + part;
        struct MinMax min_max_part = GetMinMax(array, arr_start, arr_end);

        // use pipe here
        close(file_pipe[0]);
        write(file_pipe[1], &min_max_part.min, sizeof(min_max_part.min));
        write(file_pipe[1], &min_max_part.max, sizeof(min_max_part.max));
        close(file_pipe[1]);

        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for ( i = 0; i < pnum; i++) {
    int min, max;
    //read from pipes
    file_pipe = pipe_array + (i * 2);
    close(file_pipe[1]);
    read(file_pipe[0], &min, sizeof(min));
    read(file_pipe[0], &max, sizeof(max));
    close(file_pipe[0]);

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
