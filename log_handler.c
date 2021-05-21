//
// Created by Glenn Phillips on 17/5/21.
//

// --- System Libraries ---
#include <stdio.h> // for output
#include <stdbool.h> // for booleans
#include <string.h> // for strcat
#include <stdlib.h>

// --- Project Libraries ---
#include "log_handler.h"
#include "byte_converter.h"
#include "timestamp.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// updates the log based on the specified packet
void update_log(packet_t *packet) {

    char *timestamp = get_timestamp();

    FILE *file;

    file = fopen("dns_svr.log", "a");

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

    // to ensure that log updates are timely, as specified in the spec
    fflush(file);

    // close the file
    fclose(file);

    // done with the timestamp
    free_timestamp(timestamp);
}

// --- Helper Function Implementations ---