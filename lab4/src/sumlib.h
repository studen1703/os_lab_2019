#ifndef SUMLIB_H
#define SUMLIB_H

struct SumArgs {
  int *array;
  int begin;
  int end;
  int *array_sum;
};
int Sum(const struct SumArgs *args);
#endif