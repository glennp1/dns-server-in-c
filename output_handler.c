//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  output the log entry to the file ./dns_svr.log

// --- System Libraries ---
#include <stdio.h> // for output
#include <stdbool.h> // for booleans
#include <string.h> // for concatenating strings

// --- Project Libraries ---
#include "output_handler.h"
#include "byte_converter.h"
#include "timestamp.h"

// --- Constant Definitions ---

// number of bytes reserved for the header
#define HEADER_OFFSET 12
#define AAAA_TYPE 28

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


    // store if response or request
    bool is_response = bits[0];

    // print each bit
    for (int i = 0; i < 8; i++) {
        printf("%d\n", bits[i]);
    }

    // todo display the label

    // stores the next byte to display
    int next_byte = HEADER_OFFSET;

    // store the entire label
    char entire_label[1000] = "";

    // stores the size of each label
    int label_size;

    // continue until all of the labels have been read
    while(true) {

        // store the label size
        label_size = packet->bytes[next_byte];

        // if there is no label left to print then it must be the end
        if (label_size == 0) {
            break;
        }

        // if it is not the first entry print a dot separator
        if (next_byte != HEADER_OFFSET) {
            strcat(entire_label, ".");
        }

        // print each character in the label
        for (int i = 1; i <= label_size; i++ ) {

            char label_char = packet->bytes[next_byte + i];

            strcat(entire_label, &label_char);
        }

        // increase the offset
        next_byte = next_byte + label_size + 1;
    }

    // display the label
    printf("%s\n", entire_label);

    // to store the type
    byte_t first_type_byte, second_type_byte;
    int type;

    // get the next two bytes, increment afterwards
    first_type_byte = packet->bytes[next_byte++];
    second_type_byte = packet->bytes[next_byte++];

    type = two_bytes_to_integer(first_type_byte, second_type_byte);

    // display the formatted time, this is a test
    char *timestamp = get_timestamp();

    printf("%s\n", timestamp);

    free_timestamp(timestamp);
}

// --- Helper Function Implementations ---