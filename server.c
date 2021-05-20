//
// Created by Glenn Phillips on 19/5/21.
//
// Based on example from:
// "6.1 A Simple Stream Server"
// https://beej.us/guide/bgnet/html/#a-simple-stream-client
//

// --- System Libraries ---
// todo figure out which are necessary
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

// --- Project Libraries ---
#include "server.h"
#include "packet.h"
#include "output_handler.h"

// --- Constant Definitions ---

#define CORRECT_NUM_ARGUMENTS 3
#define UPSTREAM_SERVER_IP_INDEX 1
#define UPSTREAM_SERVER_PORT_INDEX 2

#define PORT "8053"

// todo follow up?
// maximum number of connections is 100, as per the specification sheet
#define BACKLOG 100

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// sets up the socket to connect with the client, then returns the socket
int setup_client_connection();

// starts listening on the client socket, this can only be terminated via the console
void start_client_connection(int client_sockfd);

// accept and respond to any connections on the client socket
void respond_client_connection(int client_sockfd);

// sets up the socket to connect with the upstream server, then returns the socket
int setup_upstream_connection();

// starts the connection with the upstream server, then returns the socket created
int start_upstream_connection();

// sends the specified request to the upstream server via the connection specified
// by the socket
packet_t *query_upstream_connection(int upstream_connfd, packet_t *client_packet);

// --- Function Implementations ---

// takes the input from the command line interface and sets up a new server
server_t *setup_server(int argc, char *argv[]) {

    // initialise the server
    server_t *server = malloc(sizeof(server_t));

    // if the number of arguments is incorrect
    if (argc != CORRECT_NUM_ARGUMENTS) {
        // there is a problem, exit
        printf("Error: Incorrect number of arguments entered\n");
        exit(1);
    }

    // initialise upstream server ip and port
    server->upstream_server_ip = argv[UPSTREAM_SERVER_IP_INDEX];
    server->upstream_server_port = argv[UPSTREAM_SERVER_PORT_INDEX];

    return server;
}

// starts the specified server, this can only be terminated via the console
void start_server(server_t *server) {

    // todo not sure if the server should ever exit because of an error?
    //  or should it keep going?

    // DNS SERVER

    // Set up a socket so it can receive and send
    int client_sockfd = setup_client_connection();

    start_client_connection(client_sockfd);

}

void free_server(server_t *server) {
    free(server);
}

// --- Helper Function Implementations ---


// sets up the socket to connect with the client, then returns the socket
int setup_client_connection() {
    // initialise variables
    int client_sockfd;  // listen on sock_fd
    struct addrinfo hints, *servinfo, *p;
    int enable = 1;
    int rv;

    // assign the memory for hints
    memset(&hints, 0, sizeof hints);

    // instantiate hints, this provides a set of criteria for selecting
    // the socket address structures returned by getaddrinfo
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    // compile a list of appropriate socket address structures
    rv = getaddrinfo(NULL, PORT, &hints, &servinfo);

    if (rv != 0) {

        // todo change this to some different kind of error?
        //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // todo clean up the naming so this is more readable
    // iterate through the list of socket address structures and
    // bind to the first one available
    for (p = servinfo; p != NULL; p = p->ai_next) {

        // attempt to create a new socket using the current socket address structure
        client_sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        // check if creating the new socket failed
        if (client_sockfd == -1) {
            // if it did then exit
            printf("Socket creation failed\n");
            exit(1);
        }

        // attempt to set the options of the socket, this ensures that bound ports
        // and interfaces can be reused
        if (setsockopt(client_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {

            // if unsuccessful, then exit
            printf("Setting socket options failed\n");
            exit(1);
        }

        // attempt to bind the socket
        if (bind(client_sockfd, p->ai_addr, p->ai_addrlen) == -1) {

            // if unsuccessful, close this socket and continue to the next loop
            // this will cause the program to try another socket with a different
            // address structure
            close(client_sockfd);
            printf("Socket bind failed\n");
            continue;
        }
        else {

            // if successful, then we are done
            break;
        }
    }

    // now that we are done with the socket address structure it can be freed
    freeaddrinfo(servinfo);

    // if the address info of the socket does not exist
    if (p == NULL)  {

        // the bind must have failed, so exit
        printf("Socket bind failed\n");
        exit(1);
    }

    return client_sockfd;
}

// starts listening on the client socket, this can only be terminated via the console
void start_client_connection(int client_sockfd) {

    // attempt to mark the socket as listening (passive)
    if (listen(client_sockfd, BACKLOG) == -1) {

        // if unsuccessful, then exit
        printf("Socket listen failed\n");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        respond_client_connection(client_sockfd);
    }
    // NOTE: because the above loop is infinite, and we are not required to implement
    // signal handling, client_sockfd is never closed.
}

// accept and respond to any connections on the client socket
void respond_client_connection(int client_sockfd) {
    struct sockaddr_storage client_addr; // connector's address information
    socklen_t sin_size;
    int client_connfd; // new connection on client_connfd
    packet_t *client_packet;

    sin_size = sizeof client_addr;

    // attempt to accept a connection from the client and create a socket
    client_connfd = accept(client_sockfd, (struct sockaddr *)&client_addr, &sin_size);

    // if the connection was unsuccessful
    if (client_connfd == -1) {

        // display there was an error
        printf("Socket accept failed\n");

        // continue to wait for other connections
        continue;
    }

    // take the stream from the client message store it as a packet
    // this will print the packet as well
    client_packet = new_packet(client_connfd);

    // update the log accordingly
    display_output(client_packet);

    // todo forward request to upstream server
    // todo forward it to a server ipv4 (NOT IPV6????) address is the first
    //  command line argument, port is second command line argument

    int upstream_sockfd = setup_upstream_connection();

    query_upstream_connection(upstream_sockfd, client_packet);

    // todo receive response from upstream server

    // todo send the response back to the client over the SAME TCP CONNECTION
    //  separate TCP connection for each query/response with the client
    //  -- log as described

    // todo program should be ready to accept another query as soon as it has
    //  processed the previous query and response

    // todo make sure everything is freed
    // done with the packet
    free_packet(client_packet);

    // done reading the message
    close(client_connfd);
}

// sets up the socket to connect with the upstream server, then returns the socket
int setup_upstream_connection() {

    // todo

}

// starts the connection with the upstream server, then returns the socket created
int start_upstream_connection() {

    // todo

}

// sends the specified request to the upstream server via the connection specified
// by the socket
packet_t *query_upstream_connection(int upstream_connfd, packet_t *client_packet) {

}
