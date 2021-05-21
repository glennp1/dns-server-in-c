//
// Created by Glenn Phillips on 21/5/21.
//

#ifndef UPSTREAM_CONNECTION_H
#define UPSTREAM_CONNECTION_H

// --- System Libraries ---

// --- Project Libraries ---
#include "server.h"
#include "packet.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// sets up the socket to connect with the upstream server
void setup_upstream_connection(server_t *server);

// sends the specified request packet to the upstream server,
// and closes the socket once a response is received
// then returns the response packet
packet_t *send_req_to_upstream(server_t *server, packet_t *request_packet);

#endif //UPSTREAM_CONNECTION_H
