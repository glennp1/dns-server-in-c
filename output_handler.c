//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  output the log entry to the file ./dns_svr.log

// --- System Libraries ---
#include <stdio.h> // for output

// --- Project Libraries ---
#include "output_handler.h"
#include "byte_converter.h"
#include "timestamp.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

void display_output(byte_array_t *packet) {

    // todo for displaying as hex
    //  printf("%02x\n", byte);

    // print the packet length
    printf("%d\n", packet->size);

    // convert the 3rd byte in the packet to bits
    bit_t *bits = one_byte_to_bits(packet->bytes[2]);

    // print each bit
    for (int i = 0; i < 8; i++) {
        printf("%d\n", *(bits+i));
    }

    // display the formatted time, this is a test
    char *timestamp = get_timestamp();

    printf("%s\n", timestamp);

    free_timestamp(timestamp);
}

// --- Helper Function Implementations ---