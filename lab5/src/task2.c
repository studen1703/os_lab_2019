#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h> 

#include <getopt.h>
#include <pthread.h>

int total_result = 1; /* A shared variable for threads */
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

// Arguments struct
struct ThreadArgs {
    int begin;
    int end;
    int mod;
};

void* Factorial(void* _args) 
{
    struct ThreadArgs args = *(struct ThreadArgs*)_args;
    int thread_result = 1;
    int i;
    for (i = args.begin; i < args.end; i++) {
        thread_result *= i;
        thread_result %= args.mod;
    }
    
    pthread_mutex_lock(&mut);
    total_result *= thread_result;
    total_result %= args.mod;
    pthread_mutex_unlock(&mut);

    return NULL;
}

int main(int argc, char **argv) 
{
  uint32_t pnum = -1;
  uint32_t mod = -1;
  uint32_t k = -1;
  pthread_t *threads = NULL;

  while (true) 
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "k", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            if (k <= 0) {
                printf("k is a positive number\n");
                return 1;
            }
            break;
          case 1:
            pnum = atoi(optarg);
            if (pnum <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 2:
            mod = atoi(optarg);
            if (mod <= 0) {
                printf("mod is a positive number\n");
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
  if (optind < argc) 
  {
    printf("Has at least one no option argument\n");
    return 1;
  }

  threads = malloc(sizeof(pthread_t) * pnum);
  int i;
  struct ThreadArgs* args = malloc(sizeof(struct ThreadArgs) * pnum);

  for (i = 0; i < pnum; i++) 
  {
    args[i].begin = k * i / pnum + 1;
    args[i].end = k * (i + 1) / pnum + 1;
    args[i].mod = mod;
    if (pthread_create(&threads[i], NULL, Factorial, (void*)(args + i))) 
    {
        perror("pthread_create");
        exit(1);
    }
  }

  for (i = 0; i < pnum; i++) {
    if (pthread_join(threads[i], NULL)) 
    {
        perror("pthread_join");
        exit(1);
    }
  }

  printf("Result: %d\n", total_result);

  free(threads);
  free(args);

  return 0;
}
