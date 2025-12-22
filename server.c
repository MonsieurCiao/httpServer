#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>

// Server                				  	Client
// create socket
// wait for connection req	TCP connection setup		create socket,
// connect to server read request 				<-
// send request write reply				->
// read reply close 							close

// CREATE SOCKET
struct sockaddr_in server_addr;

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("error initializing socket");
    return 1;
  }
  // this is supposed to allow multiple connections
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))) {
    perror("setsocketopt(SO_REUSEADDR) failed");
  }
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int))) {
    perror("setsocketopt(SO_REUSEPORT) failed");
  }

  // Bind socket
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(3421);

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("socket binding failed");
    exit(EXIT_FAILURE);
  }
  if (listen(sockfd, 10) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }
  int clientfd;
  struct sockaddr_storage clientaddr;
  socklen_t addrSize = sizeof(clientaddr);
  if ((clientfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrSize)) <
      0) {
    perror("accepting connection failed");
  }

  printf("everything worked\n");
  return 0;
};
