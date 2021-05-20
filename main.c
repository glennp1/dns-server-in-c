//
// Created by Glenn Phillips on 10/5/21.
//

// --- System Libraries ---

// --- Project Libraries ---

#include "server.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

int main(int argc, char *argv[]) {

    server_t *server = setup_server(argc, argv);

    // todo start server
    start_server(server);

    return 0;
}

// --- Helper Function Implementations ---