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

// initialises the client address information list, that is then used to
// find a socket to bind to
struct addrinfo *setup_client_addr_info();

// accept and respond to any connections on the client socket
void accept_client_connection(server_t *server);

// processes a client connection that has been accepted by the server
void process_client_connection(server_t *server);

// --- Function Implementations ---

// sets up the socket to connect with the client
void setup_client_connection(server_t *server) {
    // for setting the options of the socket
    int enable = 1;

    // initialise client address information list
    struct addrinfo *client_info = setup_client_addr_info();

    // to iterate through the list of client info
    struct addrinfo *curr_info;

    // find a socket address structure to bind to from within client info
    for (curr_info = client_info; curr_info != NULL; curr_info = curr_info->ai_next) {

        // attempt to create a new socket using the current socket address structure
        server->client_sockfd = socket(curr_info->ai_family, curr_info->ai_socktype, curr_info->ai_protocol);

        // if creating the new socket was unsuccessful, then exit
        if (server->client_sockfd == -1) {
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
        if (bind(server->client_sockfd, curr_info->ai_addr, curr_info->ai_addrlen) == -1) {

            // if unsuccessful, continue to a different address structure
            close(server->client_sockfd);
            printf("error: client socket bind failed\n");
            continue;
        }
        // if this point is reached, then we are done
        break;
    }

    // if the current address info that was used to bind is null, then it has failed
    if (curr_info == NULL)  {
        printf("error: client socket bind failed\n");
        exit(1);
    }

    // now that we are done with the socket address structure it can be freed
    freeaddrinfo(client_info);
}

// starts listening on the client socket, this can only be terminated via the console
void listen_client_connection(server_t *server) {

    // attempt to mark the socket as listening (passive)
    if (listen(server->client_sockfd, BACKLOG) == -1) {

        // if unsuccessful, then exit
        printf("error: client socket listen failed\n");
        exit(1);
    }

    printf("waiting for client connections...\n");

    while(1) {  // main accept() loop
        accept_client_connection(server);
    }
    // NOTE: Due to the above loop being infinite, and that we are not required to implement
    // signal handling, client_sockfd is never closed.
}


// --- Helper Function Implementations ---

// initialises the client address information list, that is then used to
// find a socket to bind to
struct addrinfo *setup_client_addr_info() {

    // to store the output
    struct addrinfo *client_info;

    // clear the memory stored within hints
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);

    // instantiate hints, this provides a set of criteria for selecting
    // the socket address structures returned by getaddrinfo
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    // attempt to compile a list of appropriate socket address structures
    if (getaddrinfo(NULL, CLIENT_PORT, &hints, &client_info) != 0) {
        printf("error: no client addresses structures were found\n");
        exit(1);
    }

    // if successful return the list
    return client_info;
}

// accept and respond to any connections on the client socket
void accept_client_connection(server_t *server) {

    // to store the address of the connection that is accepted
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    // attempt to accept a connection from the client and create a socket
    server->client_connfd = accept(server->client_sockfd,
                                   (struct sockaddr *)&client_addr,
                                   &client_addr_size);

    // if the connection was unsuccessful
    if (server->client_connfd == -1) {

        // display there was an error and continue to wait for other connections
        printf("error: client socket accept failed\n");
        return;
    }
        // otherwise if it is successful, process the client connection
    else {
        process_client_connection(server);
    }
}

// processes a client connection that has been accepted by the server
void process_client_connection(server_t *server) {
    packet_t *request_packet, *response_packet;

    // take the stream from the client message store it as a packet
    request_packet = new_packet(server->client_connfd);
    print_packet_bytes(request_packet);
    update_log(request_packet);

    // if the type of the question is not AAAA
    if (!request_packet->qtype_is_aaaa) {

        // send the request back with an the rcode of 4
        change_packet_rcode(request_packet);
        write(server->client_connfd, request_packet->bytes, request_packet->length);

        // done with the packet and the socket
        free_packet(request_packet);
        close(server->client_connfd);
        return;
    }
    // otherwise if the question a correctly formed AAAA packet
    else {

        // forward the request to the upstream server and await a response
        setup_upstream_connection(server);
        response_packet = send_req_to_upstream(server, request_packet);
        printf("\nresponse:\n");
        print_packet_bytes(response_packet);
        update_log(response_packet);

        // forward the response to the client
        write(server->client_connfd, response_packet->bytes, response_packet->length);

        // done with the packets and the socket
        free_packet(request_packet);
        free_packet(response_packet);
        close(server->client_connfd);
    }
}