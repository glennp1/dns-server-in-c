//
// Created by Glenn Phillips on 10/5/21.
//

// --- System Libraries ---
#include <unistd.h> // for STDIN_FILENO

// --- Project Libraries ---
#include "input_handler.h"
#include "log_handler.h"
#include "packet.h"


// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

int main(int argc, char *argv[]) {

    // create a new packet
    packet_t *packet = new_packet(STDIN_FILENO);

    update_log(packet);

    // we are done with the packet
    free_packet(packet);

    return 0;
}

// --- Helper Function Implementations ---
