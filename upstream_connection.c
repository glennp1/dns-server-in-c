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
#include "upstream_connection.h"
#include "packet.h"
#include "log_handler.h"
#include "client_connection.h"
#include "upstream_connection.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// todo single upstream connection for now, perhaps implement multiple later
// sets up the socket to connect with the upstream server
void setup_upstream_connection(server_t *server) {

    struct sockaddr_in upstream_addr;

    server->upstream_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->upstream_sockfd == -1) {
        printf("error: upstream socket creation failed\n");
        exit(1);
    }

    // ensure that the memory of the upstream address is cleared
    bzero(&upstream_addr, sizeof(upstream_addr));

    // assign IP, PORT
    upstream_addr.sin_family = AF_INET;
    upstream_addr.sin_addr.s_addr = inet_addr(server->upstream_server_ip);
    upstream_addr.sin_port = htons(atoi(server->upstream_server_port));

    // connect the client socket to server socket
    if (connect(server->upstream_sockfd, (struct sockaddr*)&upstream_addr, sizeof(upstream_addr)) != 0) {
        printf("error: connection with the upstream server failed\n");
        exit(1);
    }

    printf("connecting to upstream server...\n");
}

// sends the specified request packet to the upstream server,
// then returns the response packet
packet_t *send_req_to_upstream(server_t *server, packet_t *request_packet) {

    // todo write function

    // todo not sure how this will work
    write(server->upstream_sockfd, request_packet->bytes, request_packet->length);

    // todo read function
    packet_t *response_packet = new_packet(server->upstream_sockfd);

    // we are done with the upstream socket
    close(server->upstream_sockfd);

    // todo this is a placeholder
    return response_packet;
}

// --- Helper Function Implementations ---

