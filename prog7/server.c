#include <stdio.h>      // Input/output 
#include <stdlib.h>     // Memory allocation and random number generation
#include <string.h>     // Copying, concatenating, and comparing strings
#include <unistd.h>     // POSIX operating system API 
#include <pthread.h>    // Multi-threaded programming including functions for creating and managing threads
#include <sys/socket.h> // Access to the low-level socket API for creating and communicating over network sockets
#include <arpa/inet.h>  // Functions for converting between different network address formats, between IP addresses

#define MAX_CLIENTS 100
#define MAX_MSG_SIZE 1024
#define BUF_SIZE 1024
#define MAX_NAME_SIZE 32

typedef struct {
  int fd;
  char username[MAX_NAME_SIZE];
}
client_t;

client_t clients[MAX_CLIENTS];
int num_clients = 0;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_msg(char * msg, int us);
void * handle_client(void * arg);

int main(int argc, char ** argv) {
  int server_fd, new_client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(struct sockaddr_in);
  pthread_t tid;
  client_t * client;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[1]));
  server_addr.sin_addr.s_addr = INADDR_ANY;
  bzero( & (server_addr.sin_zero), 8);

  if (bind(server_fd, (struct sockaddr * ) & server_addr, sizeof(struct sockaddr_in)) == -1) {
    perror("Binding failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 10) == -1) {
    perror("Listening failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Chat server started on port: %s\n", argv[1]);

  while (1) {
    if ((new_client_fd = accept(server_fd, (struct sockaddr * ) & client_addr, & addr_len)) < 0) {
      perror("Accept failed");
      exit(EXIT_FAILURE);
    }

    client = & clients[num_clients++];
    client -> fd = new_client_fd;

    pthread_create( & tid, NULL, handle_client, client);

    sleep(1);
  }

  return 0;
}

void broadcast_msg(char * msg, int us) {
  pthread_mutex_lock( & clients_mutex);

  for (int i = 0; i < num_clients; i++) {
    if (i != us) {
      write(clients[i].fd, msg, strlen(msg));
    }
  }

  pthread_mutex_unlock( & clients_mutex);
}

void * handle_client(void * arg) {
  char buffer[BUF_SIZE];
  char msg[MAX_MSG_SIZE];
  char welcome_msg[MAX_MSG_SIZE];
  char exit_msg[MAX_MSG_SIZE];
  int count;
  size_t total_length;
  size_t buffer_length;

  client_t * client = (client_t * ) arg;

  // Clear buffer
  memset(buffer, 0, sizeof(buffer));

  // First read should be the username
  count = read(client -> fd, client -> username, sizeof(client -> username) - 1);
  if (count > 0) {
    client -> username[count] = '\0';
  } else {
    printf("Failed to read username.\n");
    return NULL;
  }

  printf("%s has joined\n", client -> username);
  fflush(stdout);

  // Send a welcome message
  snprintf(welcome_msg, sizeof(welcome_msg), "Welcome %s! If you want to quit, type \\q\n", client -> username);
  if (write(client -> fd, welcome_msg, strlen(welcome_msg)) < 0) {
    perror("Write to descriptor failed");
    exit(EXIT_FAILURE);
  }

  // Echo client's messages to all clients
  while ((count = read(client -> fd, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[count] = '\0';
    if (strncmp(buffer, "\\q", 2) == 0) break;

    total_length = strlen(client -> username) + strlen(buffer) + 3;
    if (total_length > sizeof(msg)) {
      fprintf(stderr, "Message is too long for buffer\n");
      // Truncate buffer to fit into msg
      buffer_length = sizeof(msg) - strlen(client -> username) - 3;
      buffer[buffer_length] = '\0';
    }
    snprintf(msg, sizeof(msg), "%.30s: %.900s", client -> username, buffer);

    broadcast_msg(msg, client -> fd);
  }

  // Announce client's departure
  snprintf(exit_msg, sizeof(exit_msg), "%s has quit\n", client -> username);
  broadcast_msg(exit_msg, client -> fd);

  close(client -> fd);

  // Remove client from the clients array
  pthread_mutex_lock( & clients_mutex);
  for (int i = 0; i < num_clients; ++i) {
    if (clients[i].fd == client -> fd) {
      // Shift remaining clients down the array
      for (int j = i; j < num_clients - 1; ++j) {
        clients[j] = clients[j + 1];
      }
      break;
    }
  }
  num_clients--;
  pthread_mutex_unlock( & clients_mutex);

  return NULL;
}