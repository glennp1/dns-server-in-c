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
#include "log_handler.h"
#include "client_connection.h"
#include "upstream_connection.h"

// --- Constant Definitions ---

#define CORRECT_NUM_ARGUMENTS 3
#define UPSTREAM_SERVER_IP_INDEX 1
#define UPSTREAM_SERVER_PORT_INDEX 2

// --- Type Definitions ---

// --- Helper Function Prototypes ---

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

    // todo for upstream and client
    // set up a socket to respond to client requests,
    // details are stored within server
    setup_client_connection(server);

    // start listening on the client socket
    listen_client_connection(server);


    // todo for upstream only
//    packet_t *request_packet = new_packet(STDIN_FILENO);
//    print_packet_bytes(request_packet);
//    update_log(request_packet);
//
//    setup_upstream_connection(server);
//    packet_t *response_packet = send_req_to_upstream(server, request_packet);
//    printf("\nresponse:\n");
//    print_packet_bytes(response_packet);
//    update_log(response_packet);



}

void free_server(server_t *server) {
    free(server);
}

// --- Helper Function Implementations ---





