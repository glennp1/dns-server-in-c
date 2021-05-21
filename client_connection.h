//
// Created by Glenn Phillips on 21/5/21.
//

#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

// --- System Libraries ---

// --- Project Libraries ---
#include "server.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// sets up the socket to connect with the client
void setup_client_connection(server_t *server);

// starts listening on the client socket, this can only be terminated via the console
void listen_client_connection(server_t *server);

#endif //CLIENT_CONNECTION_H
