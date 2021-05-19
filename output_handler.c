//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  output the log entry to the file ./dns_svr.log

// --- System Libraries ---
#include <stdio.h> // for output
#include <stdbool.h> // for booleans


#include <string.h> // for strcat

// todo remove
#include <stdlib.h>

// --- Project Libraries ---
#include "output_handler.h"
#include "byte_converter.h"
#include "timestamp.h"

// --- Constant Definitions ---

// number of bytes reserved for the header
#define AAAA_QTYPE 28 // for qtype

// todo might not be required
#define IN_QCLASS 1 // for qclass

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

void display_output(packet_t *packet) {

    char *timestamp = get_timestamp();

    FILE *file;

    file = fopen("dns_svr.log", "a");

    fprintf(file, "%s ", timestamp);

    if (packet->qtype != AAAA_QTYPE) {
        fprintf(file, "unimplemented request\n");
    }
    else {
        if (packet->is_response) {
            fprintf(file, "%s is at %s\n", packet->url, packet->ip_address);
        }
    }

    // todo not sure if this is necessary
    // to ensure that log updates are timely, as specified in the spec
    // fflush(file);

    // close the file
    fclose(file);



    // done with the timestamp amd the packet
    free_timestamp(timestamp);
    free_packet(packet);
}

// --- Helper Function Implementations ---