//
// Created by Glenn Phillips on 18/5/21.
//

// --- System Libraries ---
#include <stdlib.h> // for malloc

// --- Project Libraries ---
#include "packet.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// todo new packet
packet_t *new_packet(byte_t *bytes, int length) {

    // initialise the array
    packet_t *packet = malloc(sizeof(packet_t));

    // initialise the bytes and length
    packet->bytes = bytes;
    packet->length = length;

    return packet;
}

// todo free packet
void free_packet(packet_t *packet) {

    // free the bytes
    free(packet->bytes);

    // length does not need to be freed

    // free the packet itself
    free(packet);
}

// --- Helper Function Implementations ---

