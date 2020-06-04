#ifndef LIB_H
#define LIB_H
#include <netinet/in.h>
struct FactorialArgs {
  uint64_t begin;
  uint64_t end;
  uint64_t mod;
};
struct Server {
  char ip[255];
  int port;
};
bool ConvertStringToUI64(const char *str, uint64_t *val);
uint64_t Factorial(const struct FactorialArgs *args);

#endif