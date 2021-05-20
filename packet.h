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
    int url_size;
    char *url;
    bool qtype_is_aaaa; // for question type
    bool first_atype_is_aaaa; // for first answer type
    int rdlength;
    char *ip_address;
};

// --- Function Prototypes ---

// creates and returns a new packet based on the specified input
packet_t *new_packet(int input_file);

// frees the specified packet and all of its associated memory
void free_packet(packet_t *packet);

#endif //PACKET_H
