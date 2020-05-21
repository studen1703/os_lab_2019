#include "sumlib.h"

#include <stdio.h>
#include <stdlib.h>
int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here 
  int i = 0;
  for(i = args->begin; i<args->end; i++)
    sum += args->array[i];
  printf("d = %d\n",sum);
  return sum;
}