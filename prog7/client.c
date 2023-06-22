// Header Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_MSG_SIZE 1024

int server_fd;

void * send_to_server(void * arg);
void * recv_from_server(void * arg);

int main(int argc, char * argv[]) {
  struct sockaddr_in server_addr;
  pthread_t send_thread, recv_thread;
  char username[100];

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  printf("Enter username: ");
  fgets(username, sizeof(username), stdin);
  username[strcspn(username, "\n")] = 0;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));

  if (inet_pton(AF_INET, argv[1], & server_addr.sin_addr) <= 0) {
    perror("Invalid address or address not supported");
    exit(EXIT_FAILURE);
  }

  if (connect(server_fd, (struct sockaddr * ) & server_addr, sizeof(server_addr)) < 0) {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }

  write(server_fd, username, strlen(username));

  pthread_create( & send_thread, NULL, send_to_server, NULL);
  pthread_create( & recv_thread, NULL, recv_from_server, NULL);

  pthread_join(send_thread, NULL);
  pthread_join(recv_thread, NULL);

  return 0;
}

void * send_to_server(void * arg) {
  char buffer[MAX_MSG_SIZE];

  while (1) {
    fgets(buffer, MAX_MSG_SIZE, stdin);
    if (buffer[0] == '\\') {
      if (buffer[1] == 'q') {
        close(server_fd);
        exit(EXIT_SUCCESS);
      } else {
        printf("Unrecognized command: %s\n", buffer);
        continue;
      }
    }
    write(server_fd, buffer, strlen(buffer));
  }
}

void * recv_from_server(void * arg) {
  char buffer[MAX_MSG_SIZE];
  int count;

  while ((count = read(server_fd, buffer, MAX_MSG_SIZE - 1)) > 0) {
    buffer[count] = '\0';
    printf("%s", buffer);
  }

  close(server_fd);
  exit(EXIT_SUCCESS);
}