#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sumlib.h"

#include <getopt.h>
#include <pthread.h>
#include "utils.h"//"/projects/os_lab_2019/lab3/src/utils.h"

/*struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here 
  int i = 0;
  for(i = args->begin; i<args->end; i++)
    sum += args->array[i];
  printf("d = %d\n",sum);
  return sum;
}
*/
void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  int sum = Sum(sum_args);
  *(sum_args->array_sum) = sum;
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  pthread_t *threads = NULL;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"threads_num", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "t", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            threads_num = atoi(optarg);
            if (threads_num <= 0) {
                printf("threads_num is a positive number\n");
                return 1;
            }
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
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

  /*
   * TODO:
   * your code here
   * Generate array here
   */
  threads = malloc(sizeof(pthread_t) * threads_num);
  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  struct SumArgs *args =  malloc(sizeof(struct SumArgs) * threads_num);
  uint32_t i;
  int *array_sum = malloc(sizeof(int) * threads_num); //массив для сумм потоков

  for ( i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = array_size*i/threads_num;
    args[i].end =  array_size*(i+1)/threads_num;
    args[i].array_sum = array_sum + i;
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)(args+i))) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;

  for ( i = 0; i < threads_num; i++) {
    //pthread_join(threads[i], (void **)&sum);
    pthread_join(threads[i], NULL);
    total_sum += array_sum[i];
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  free(args);
  free(array_sum);
  free(threads);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n", elapsed_time);
  return 0;
}
