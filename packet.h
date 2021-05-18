//
// Created by Glenn Phillips on 18/5/21.
//

#ifndef PACKET_H
#define PACKET_H

// --- System Libraries ---

#include <stdbool.h>

// --- Project Libraries ---
#include "byte_converter.h"

// --- Constant Definitions ---

// --- Type Definitions ---

typedef struct packet_s packet_t;

struct packet_s {
    byte_t *bytes;
    int length;
    bool is_response;
    int url;
    int qtype;
    int rdlength;
    char *ip_address;
};

// --- Function Prototypes ---

// todo new packet
packet_t *new_packet(byte_t *bytes, int length);

// todo free packet
void free_packet(packet_t *packet);

#endif //PACKET_H
