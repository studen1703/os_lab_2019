#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
    char *adr = NULL; 
    int port = -1;
    int bufsize = -1;
    while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"adr", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {"bufsize", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        asprintf(&adr, "%s", optarg);
      case 1:
        port = atoi(optarg);
        // TODO: your code here
        break;
      case 2:
        bufsize = atoi(optarg);
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

  if (adr == NULL || port == -1 || bufsize == -1) {
    fprintf(stderr, "Using: %s --adr 127.0.0.1 --port 1234 --bufsize 100\n", &adr);
    return 1;
  }
  int sockfd, n;
  char *sendline = (char*)malloc(sizeof(char) * bufsize);
  char *recvline = (char*)malloc(sizeof(char) * (bufsize+1));
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_pton(AF_INET, adr, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    free(adr);
    exit(1);
  }
  free(adr);
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, bufsize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, bufsize, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
  free(sendline);
  free(recvline);
}
