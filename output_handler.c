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


// todo might not be required
#define IN_QCLASS 1 // for qclass

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

void display_output(packet_t *packet) {

    char *timestamp = get_timestamp();

    FILE *file;

    file = fopen("dns_svr.log", "a");

    // todo not sure if this should be at the start
    // qtype aaaa?????

    // if the packet is a response
    if (packet->is_response) {

        // if the type of the first answer is AAAA
        if (packet->first_atype_is_aaaa) {
            // add the response to the log file
            fprintf(file, "%s %s is at %s\n", timestamp, packet->url, packet->ip_address);
        }
        // otherwise if the type of the first answer is not AAAA
        else {
            // do not add a log entry
        }
    }
    // otherwise if the packet is a request
    else {
        // add the request to the log file
        fprintf(file, "%s requested %s\n", timestamp, packet->url);

        // if the type of the question is not AAAA
        if (!packet->qtype_is_aaaa) {

            // add that the request is unimplemented to the log file
            fprintf(file, "%s unimplemented request\n", timestamp);
        }
    }


    // todo not sure if this is necessary
    // to ensure that log updates are timely, as specified in the spec
    // fflush(file);

    // close the file
    fclose(file);

    // done with the timestamp
    free_timestamp(timestamp);
}

// --- Helper Function Implementations ---