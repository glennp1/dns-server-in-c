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
#include <signal.h>

// --- Project Libraries ---
#include "server.h"
#include "packet.h"
#include "output_handler.h"

// --- Constant Definitions ---

#define PORT "8053"

// todo follow up?
// maximum number of connections is 100, as per the specification sheet
#define BACKLOG 100

// todo update this later? not sure what max would be?
// max number of characters that will be read from the client
#define CLIENT_INPUT_BUFFER_SIZE 150

// --- Type Definitions ---

// --- Helper Function Prototypes ---

void sigchld_handler(int s);

void *get_in_addr(struct sockaddr *sa);

// --- Function Implementations ---

// todo currently receives some input from a client and prints it to stdout
void start_server(char *server_address, char *server_port) {

    // todo not sure if the server should ever exit because of an error?
    //  or should it keep going?

    // DNS SERVER

    // Set up a socket so it can receive and send

    // Receives requests containing an domain name

    // Looks up the IP address with the server (from std in)

    // Returns a response containing the IP address

    // todo accept a dns query over TCP on port 8053

    // todo forward it to a server ipv4 (NOT IPV6????) address is the first
    //  command line argument, port is second command line argument

    // todo not sure how to test

    // todo send the response back to the client over the SAME TCP CONNECTION
    //  separate TCP connection for each query/response with the client
    //  -- log as described

    // todo program should be ready to accept another query as soon as it has
    //  processed the previous query and response

    // initialise variables
    int sockfd;  // listen on sock_fd
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

    // iterate through the list of socket address structures and
    // bind to the first one available
    for (p = servinfo; p != NULL; p = p->ai_next) {

        // attempt to create a new socket using the current socket address structure
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        // check if creating the new socket failed
        if (sockfd == -1) {
            // if it did then exit
            printf("Socket creation failed\n");
            exit(1);
        }

        // attempt to set the options of the socket, this ensures that bound ports
        // and interfaces can be reused
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {

            // if unsuccessful, then exit
            printf("Setting socket options failed\n");
            exit(1);
        }

        // attempt to bind the socket
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {

            // if unsuccessful, close this socket and continue to the next loop
            // this will cause the program to try another socket with a different
            // address structure
            close(sockfd);
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

    // todo SPLIT

    struct sockaddr_storage client_addr; // connector's address information
    socklen_t sin_size;
    int client_sockfd; // new connection on client_sockfd
    packet_t *client_packet;

    // attempt to mark the socket as listening (passive)
    if (listen(sockfd, BACKLOG) == -1) {

        // if unsuccessful, then exit
        printf("Socket listen failed\n");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof client_addr;

        // attempt to accept a connection from the client and create a socket
        client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

        // if the connection was unsuccessful
        if (client_sockfd == -1) {

            // display there was an error
            printf("Socket accept failed\n");

            // continue to wait for other connections
            continue;
        }

        // take the stream from the client message store it as a packet
        // this will print the packet as well
        client_packet = new_packet(client_sockfd);

        // update the log accordingly
        display_output(client_packet);

        // todo forward request to upstream server

        // todo receive response from upstream server

        // todo forward response to client

        // todo done, make sure we free everything

        // todo make sure everything is freed
        // done with the packet
        free_packet(client_packet);

        // done reading the message
        close(client_sockfd);
    }

    // NOTE: because the above loop is infinite, and we are not required to implement
    // signal handling, sockfd is never closed.
}

// --- Helper Function Implementations ---

