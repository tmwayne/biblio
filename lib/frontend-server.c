//
// -----------------------------------------------------------------------------
// frontend-server.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include <stdio.h>             // printf, sscanf
#include <stdlib.h>            // EXIT_FAILURE
#include <string.h>            // strlen
#include <readline/readline.h> // readline

#include <signal.h>            // signal
#include <unistd.h>            // read, close
#include <sys/socket.h>        // socket, connect, recv, shutdown, send
#include <netinet/in.h>        // sockaddr_in
#include <arpa/inet.h>         // inet_aton, ntohs
#include <pthread.h>           // pthread_create, pthread_exit

#include "session.h"           // command_func
#include "common-string.h"
#include "mem.h"
#include "assert.h"

#define FRONTEND_TYPE "server"
#define PORT 6543
#define MAXBUF 256

typedef struct sock_info {
  int sockfd;
  struct sockaddr_in addr;
} *Sock_info;

static Sock_info host;

typedef void *(*void_ptr_func)(void *); // to cast function for pthread_create

static void exit_nicely(char *msg);
static void handle_sigint(int sig);
void *handle_connection(Session_T session);
char *readsocket(Sock_info, char *msg);

char *readsocket(Sock_info client, char *msg) {

  assert(client && msg);

  char *buf = CALLOC(MAXBUF, sizeof(char));
  int len = 0;

  // Send message
  if (send(client->sockfd, msg, strlen(msg), 0) == -1) exit_nicely("send");

  // Get response
  if ((len = recv(client->sockfd, buf, MAXBUF, 0)) == -1) exit_nicely("recv");
  buf[len] = 0;

  return strtrim(buf);

}

void server_interactive(void *session) {

  Session_T _session = session;

  while (1) {

    // Accept connection
    Sock_info client = malloc(sizeof(struct sock_info));
    socklen_t sin_size = sizeof(client->addr);
    client->sockfd = accept(host->sockfd,
      (struct sockaddr *) &client->addr, &sin_size);

    printf("Connection made from %s:%d\n", inet_ntoa(client->addr.sin_addr),
      ntohs(client->addr.sin_port));
    Session_T user_session = Session_newuser(_session, client);

    // Create thread to handle connection
    pthread_t thread;
    if (pthread_create(&thread, NULL, (void_ptr_func) handle_connection, user_session)) {
      fprintf(stderr, "Unable to create thread.\n");
      continue;
    }

  }

}


// void *handle_connection(Sock_info client) {
void *handle_connection(Session_T session) {

  Sock_info client = session->data;

  // Initialize message buffer
  char buf[MAXBUF];
  memset(buf, 0, sizeof(buf));
  size_t len;

  // Event loop
  while ((len = recv(client->sockfd, buf, MAXBUF, 0)) > 0) {
    
    buf[len] = 0; // ensure message ends with a null-terminator

    // Process request
    if (strncasecmp(buf, "add", 3) == 0)
      ((command_func) Dict_get(session->commands, "add"))(session);

    // snprintf(buf, MAXBUF, "Request received!\n");
    // if (send(client->sockfd, buf, strlen(buf), 0) == -1) exit_nicely("send");
    break;

  }

  // Free resources
  close(client->sockfd);
  free(client);
  Session_free(&session);

  pthread_exit(NULL);
  
} 

void Server_init() {

  host = CALLOC(1, sizeof(struct sock_info));

  host->addr.sin_family = AF_INET;
  host->addr.sin_port = htons(PORT);
  host->addr.sin_addr.s_addr = INADDR_ANY;

  // Create socket
  if ((host->sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) exit_nicely("socket");

  // Bind socket to port
  if (bind(host->sockfd, (struct sockaddr *) &host->addr, sizeof(host->addr)) == -1)
    exit_nicely("bind");

  // Listen for connections
  if (listen(host->sockfd, 20) == -1) exit_nicely("listen");

  // Shutdown upon receiving SIGINT
  signal(SIGINT, handle_sigint);

}

char *server_pick_topic(Dataframe_T topics, void *session) {

  Sock_info client = ((Session_T) session)->data;
  assert(topics);

  // Variables
  char *buf;
  int selection;
  char *topic = NULL;

  // Print topics
  printf("Found the following topics\n");
  for (int row=0; row < Dataframe_nrows(topics); row++)
    printf("%d) %s\n", row, Dataframe_getval(topics, row, 0));

  // Prompt user to select topic
  // if ((buf = readline("\nSelect a topic to view articles: "))) {
  if ((buf = readsocket(client, "\nSelect a topic to view articles: "))) {
    if (1 == sscanf(buf, "%d", &selection)) {
      char *val = Dataframe_getval(topics, selection, 0);
      int len = strlen(val)+1;
      topic = ALLOC(len * sizeof(char));
      snprintf(topic, len, val);
    } else {
      fprintf(stderr, "Invalid selection...\n");
      exit(EXIT_FAILURE); 
    }
    FREE(buf);
  }

  return topic;

}

int server_pick_article(Dataframe_T articles, char *topic, void *session) {

  Sock_info client = ((Session_T) session)->data;
  assert(articles && topic);

  // Variables
  char *buf;
  int selection;
  int article_id = 0;

  // List articles
  // printf("\nFound %d articles on %s\n", Dataframe_nrows(articles), topic);

  for (int row=0; row < Dataframe_nrows(articles); row++) {
    char *title = Dataframe_getval(articles, row, 1);
    char *author = Dataframe_getval(articles, row, 2);
    char *source = Dataframe_getval(articles, row, 3);
    printf("%d) ", row);
    if (strlen(title)) {
      printf("\"%s\"", title);
      if (strlen(author)) printf(" - %s", author);
    } else {
      printf(source);
    }
    printf("\n");
  }

  // Prompt user to select article
  // if ((buf = readline("\nSelect article to read: "))) {
  if ((buf = readsocket(client, "\nSelect article to read: "))) {
    if (1 != sscanf(buf, "%d", &selection)) {
      fprintf(stderr, "Invalid selection...\n");
      exit(EXIT_FAILURE);
    } 
    FREE(buf);
  }

  // List article source
  char *source = Dataframe_getval(articles, selection, 3);
  if (strlen(source)) printf("Source: %s\n", source);
  else printf("No source listed\n");

  // Prompt user to mark as read
  // if ((buf = readline("\nMark article as read? "))) {
  if ((buf = readsocket(client, "\nMark article as read? "))) {
    // if (strncasecmp(buf, "y", 1) == 0)
    if (strmatch(buf, "y"))
      article_id = atoi(Dataframe_getval(articles, selection, 0));
    FREE(buf);
  }

  return article_id;

}

Article_T server_add_article(void *session) {

  Article_T article = Article_new();

  Sock_info client = ((Session_T) session)->data;

  // char *msg = "Multi-user mode is working!\n";
  // if (send(client->sockfd, msg, strlen(msg), 0) == -1) exit_nicely("send");

  // article->topic = strdup("server");
  // article->title = strdup("test");
  // article->author = strdup("is");
  // article->source = strdup("working");
  
  article->topic = readsocket(client, "Topic: ");
  article->title = readsocket(client, "Title: ");
  article->author = readsocket(client, "Author: ");
  article->source = readsocket(client, "Source: ");

  return article;

}

void server_print_string(char *str, void *session) {

  printf(str);

}

void server_free(){

  if(host->sockfd) shutdown(host->sockfd, SHUT_RDWR);
  exit(EXIT_SUCCESS);

}

Frontend_T server_frontend_init() {

  Frontend_T server_frontend = Frontend_new();

  server_frontend->interactive = server_interactive;
  server_frontend->pick_topic = server_pick_topic;
  server_frontend->pick_article = server_pick_article;
  server_frontend->add_article = server_add_article;
  server_frontend->print_string = server_print_string;
  server_frontend->free = server_free;
  server_frontend->args = 0;

  Server_init();

  return server_frontend;

}

void register_interface(Dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);

  Entry_T entry = Entry_new(plugin_path, (void *(*)()) server_frontend_init);
  Dict_set(registry, FRONTEND_TYPE, entry);

}  

static void exit_nicely(char *msg) {
  perror(msg);
  server_free();
}

static void handle_sigint(int sig) {
  fprintf(stderr, "Shutting down server...\n");
  server_free();
}

#ifdef FRONTEND_CONSOLE_DEBUG
int main() {

}
#endif
