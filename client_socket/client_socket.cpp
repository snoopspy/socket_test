#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd == -1) {
    printf("%s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  struct sockaddr_in addr;
  memset((void*)&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(0x3D496FEE); // 61.73.111.238;
  addr.sin_port = htons(80);

  if (connect(sd, (sockaddr*)&addr, sizeof(addr)) == -1) {
    printf("%s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  const char* http_request = "GET / HTTP/1.1\r\nHost: test.gilgil.net\r\n\r\n";
  if (send(sd, http_request, strlen(http_request), 0) == -1) {
    printf("%s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  while (true) {
    char buf[256];
    int res = recv(sd, buf, 255, 0);
    if (res == -1 || res == 0) break;
    buf[res] = '\0';
    printf("%s", buf);
  }

  close(sd);
  return EXIT_SUCCESS;
}
