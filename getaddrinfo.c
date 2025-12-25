#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h> // Defines sockaddr_in and sockaddr_in6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int ac, char **av) {
  struct addrinfo hints; // filters for getaddrinfo
  struct addrinfo *res;
  struct addrinfo *r;
  int status;
  char buffer[INET6_ADDRSTRLEN];

  if (ac != 2) {
    fprintf(stderr, "usage: ./a hostname\n");
    return 1;
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(av[1], 0, &hints, &res);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
  }
  printf("IP address for %s\n", av[1]);

  r = res;
  while (r != NULL) {
    void *addr;
    if (r->ai_family == AF_INET) {
      struct socketaddr_in *ipv4 = (struct socketaddr_in *)r->ai_addr;
      inet_ntop(r->ai_family, &(ipv4->sin_addr), buffer, sizeof buffer);
      printf("IPv4: %s\n", buffer);
    } else {
      struct socketaddr_in6 *ipv6 = (struct socketaddr_in6 *)r->ai_addr;
      inet_ntop(r->ai_family, &(ipv6->sin6_addr), buffer, sizeof buffer);
      printf("IPv6: %s\n", buffer);
    }
  }
}
