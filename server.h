//
// Created by Glenn Phillips on 19/5/21.
//

#ifndef SERVER_H
#define SERVER_H

// --- System Libraries ---

// --- Project Libraries ---

// --- Constant Definitions ---

// --- Type Definitions ---

typedef struct server_s server_t;

struct server_s {
    char *upstream_server_ip;
    char *upstream_server_port;
    int client_sockfd; // listen on client_sockfd
    int client_connfd; // new connection on client_connfd
    int upstream_sockfd; // connect on upstream_sockfd
};

// --- Function Prototypes ---

// takes the input from the command line interface and sets up a new server
server_t *setup_server(int argc, char *argv[]);

// starts the specified server, this can only be terminated via the console
void start_server(server_t *server);

// frees the server and its associated data
void free_server(server_t *server);

#endif //SERVER_H
