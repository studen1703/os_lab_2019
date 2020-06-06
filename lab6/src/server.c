#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "lib.h"
#include <pthread.h>
int globalResult = 1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

void *ThreadFactorial(void *args) {
    struct FactorialArgs *fargs = (struct FactorialArgs *)args;
    uint64_t f = Factorial(fargs);
    pthread_mutex_lock(&mut);
    globalResult *= f;
    globalResult %= fargs->mod;
    pthread_mutex_unlock(&mut);
    return NULL;
}

int main(int argc, char **argv) {
  int tnum = -1;
  int port = -1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"port", required_argument, 0, 0},
                                      {"tnum", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        port = atoi(optarg);
        // TODO: your code here
        break;
      case 1:
        tnum = atoi(optarg);
        // TODO: your code here
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Unknown argument\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (port == -1 || tnum == -1) {
    fprintf(stderr, "Using: %s --port 20001 --tnum 4\n", argv[0]);
    return 1;
  }

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    fprintf(stderr, "Can not create server socket!");
    return 1;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons((uint16_t)port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

  int err = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
  if (err < 0) {
    fprintf(stderr, "Can not bind to socket!");
    return 1;
  }
  printf("!!!!!!!!!!!!before %d\n", __LINE__);
  err = listen(server_fd, 128);
  if (err < 0) {
    fprintf(stderr, "Could not listen on socket\n");
    return 1;
  }
  printf("!!!!!!!!!!!!!after %d\n", __LINE__);
  printf("Server listening at %d\n", port);

  while (true) {
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    printf("!!!!!!!!!!!!! %d\n", __LINE__);
    int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
    printf("!!!!!!!!!!!!! %d\n", __LINE__);
    if (client_fd < 0) {
      fprintf(stderr, "Could not establish new connection\n");
      continue;
    }

    while (true) {
      unsigned int buffer_size = sizeof(uint64_t) * 3;
      char from_client[buffer_size];
      int read = recv(client_fd, from_client, buffer_size, 0);

      if (!read)
        break;
      if (read < 0) {
        fprintf(stderr, "Client read failed\n");
        break;
      }
      if (read < buffer_size) {
        fprintf(stderr, "Client send wrong data format\n");
        break;
      }

      pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * tnum);

      uint64_t begin = 1;
      uint64_t end = 0;
      uint64_t mod = 0;
      memcpy(&begin, from_client, sizeof(uint64_t));
      memcpy(&end, from_client + sizeof(uint64_t), sizeof(uint64_t));
      memcpy(&mod, from_client + 2 * sizeof(uint64_t), sizeof(uint64_t));

      fprintf(stdout, "Receive: %llu %llu %llu\n", begin, end, mod);

      struct FactorialArgs *args = (struct FactorialArgs*)malloc(sizeof(struct FactorialArgs) * tnum);
      uint32_t i;
      
      for ( i = 0; i < tnum; i++) {
        // TODO: parallel somehow
        args[i].begin = begin + (end - begin)*i/tnum;
        args[i].end = begin + (end - begin)*(i+1)/tnum;
        args[i].mod = mod;

        if (pthread_create(&threads[i], NULL, ThreadFactorial,
                           (void *)&args[i])) {
          printf("Error: pthread_create failed!\n");
          return 1;
        }
      }
      for ( i = 0; i < tnum; i++) {
        pthread_join(threads[i], NULL);
      }

      char buffer[sizeof(globalResult)];
      memcpy(buffer, &globalResult, sizeof(globalResult));
      printf("server send: %d\n", globalResult);
      err = send(client_fd, buffer, sizeof(globalResult), 0);
      if (err < 0) {
        fprintf(stderr, "Can't send data to client\n");
        break;
      }
      
    free(threads);
    free(args);
    }

    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
    
  }

  return 0;
}
