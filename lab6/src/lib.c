#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "lib.h"
#include <pthread.h>


bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}
uint64_t Factorial(const struct FactorialArgs *args) {
    uint64_t ans = 1;
    printf("beginF: %d\n", args->begin);
    printf("endF: %d\n", args->end);
    printf("modF: %d\n", args->mod);
  // TODO: your code here
    int i;
    for (i = args->begin; i < args->end; i++) {
        ans *= i;
        ans %= args->mod;
    }
    printf("ans: %d\n", ans);
  return ans;
}