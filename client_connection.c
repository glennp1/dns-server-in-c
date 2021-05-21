//
// Created by Glenn Phillips on 21/5/21.
//

// --- System Libraries ---
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
#include "client_connection.h"
#include "packet.h"
#include "log_handler.h"
#include "client_connection.h"
#include "upstream_connection.h"

// --- Constant Definitions ---

#define CLIENT_PORT "8053"

// todo follow up?
// maximum number of connections is 100, as per the specification sheet
#define BACKLOG 100

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// sets up the socket to connect with the client
void setup_client_connection(server_t *server) {
    // initialise variables
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
    rv = getaddrinfo(NULL, CLIENT_PORT, &hints, &servinfo);

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
        server->client_sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        // check if creating the new socket failed
        if (server->client_sockfd == -1) {
            // if it did then exit
            printf("error: client socket creation failed\n");
            exit(1);
        }

        // attempt to set the options of the socket, this ensures that bound ports
        // and interfaces can be reused
        if (setsockopt(server->client_sockfd, SOL_SOCKET, SO_REUSEADDR,
                       &enable, sizeof(int)) < 0) {

            // if unsuccessful, then exit
            printf("error: setting client socket options failed\n");
            exit(1);
        }

        // attempt to bind the socket
        if (bind(server->client_sockfd, p->ai_addr, p->ai_addrlen) == -1) {

            // if unsuccessful, close this socket and continue to the next loop
            // this will cause the program to try another socket with a different
            // address structure
            close(server->client_sockfd);
            printf("error: client socket bind failed\n");
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
        printf("error: client socket bind failed\n");
        exit(1);
    }
}

// starts listening on the client socket, this can only be terminated via the console
void start_client_connection(server_t *server) {

    // attempt to mark the socket as listening (passive)
    if (listen(server->client_sockfd, BACKLOG) == -1) {

        // if unsuccessful, then exit
        printf("error: client socket listen failed\n");
        exit(1);
    }

    printf("waiting for client connections...\n");

    while(1) {  // main accept() loop
        respond_client_connection(server);
    }
    // NOTE: Due to the above loop being infinite, and that we are not required to implement
    // signal handling, client_sockfd is never closed.
}

// accept and respond to any connections on the client socket
void respond_client_connection(server_t *server) {
    struct sockaddr_storage client_addr; // connector's address information
    socklen_t sin_size;
    packet_t *request_packet, *response_packet;

    sin_size = sizeof client_addr;

    // attempt to accept a connection from the client and create a socket
    server->client_connfd = accept(server->client_sockfd, (struct sockaddr *)&client_addr, &sin_size);

    // if the connection was unsuccessful
    if (server->client_connfd == -1) {

        // display there was an error
        printf("error: client socket accept failed\n");

        // continue to wait for other connections
        return;
    }

    // take the stream from the client message store it as a packet
    // this will print the packet as well
    request_packet = new_packet(server->client_connfd);

    // update the log accordingly
    update_log(request_packet);

    // if the type of the question is not AAAA
    if (!request_packet->qtype_is_aaaa) {

        // change the rcode of the request to 4
        change_packet_rcode(request_packet);

        // then send a request back with rcode 4
        write(server->client_connfd, request_packet->bytes, request_packet->length);

        // done with the packet
        free_packet(request_packet);

        // done reading the message
        close(server->client_connfd);
        return;

    }

    // todo forward request to upstream server
    // todo forward it to a server ipv4 (NOT IPV6????) address is the first
    //  command line argument, port is second command line argument

    // todo

    // set up a socket to query the upstream server,
    // details are stored within server


    // todo receive response from upstream server

    // todo send the response back to the client over the SAME TCP CONNECTION
    //  separate TCP connection for each query/response with the client
    //  -- log as described

    // todo program should be ready to accept another query as soon as it has
    //  processed the previous query and response

    // todo make sure everything is freed
    // done with the packet
    free_packet(request_packet);

    free_packet(response_packet);

    // done reading the message
    close(server->client_connfd);
    return;
}

// --- Helper Function Implementations ---

