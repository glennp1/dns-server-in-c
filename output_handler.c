//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  output the log entry to the file ./dns_svr.log

// --- System Libraries ---
#include <stdio.h> // for output
#include <stdbool.h> // for booleans

// todo remove later
#include <stdlib.h> // for free

// --- Project Libraries ---
#include "output_handler.h"
#include "byte_converter.h"
#include "timestamp.h"

// --- Constant Definitions ---

// number of bytes reserved for the header
#define HEADER_OFFSET 12
#define AAAA_TYPE 28
#define NULL_BYTE '\0'
#define DOT_SEPARATOR '.'

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

void display_output(byte_array_t *packet) {

    // todo for displaying as hex
    //  printf("%02x\n", byte);

    // todo store if response or request

    // convert the 3rd byte in the packet to bits
    bit_t *bits = one_byte_to_bits(packet->bytes[2]);

     // store if response or request
     bool is_response = bits[0];

     // todo remove?

//    // print each bit
//    for (int i = 0; i < 8; i++) {
//        printf("%d\n", bits[i]);
//    }

    // todo change this to separate function
    // done with bits
    free(bits);

    // todo display the label

    // stores the url size
    int url_size = 0;

    // stores the size of a specific label
    int label_size;

    // used to iterate through the current packet
    // set to the end of the header by default
    int packet_index = HEADER_OFFSET;

    // calculate the url size
    while(1) {
        // update the url size
        label_size = packet->bytes[packet_index];
        url_size += label_size;

        // if the label has a size of zero it must be the end of the url
        if (label_size == 0) {
            break;
        }

        // leave space for the dot separators and the null byte at the end
        url_size++;

        // increase the packet index by the label size plus one
        packet_index = packet_index + label_size + 1;
    }

    // stores the url
    char *url = malloc(url_size * sizeof(char));

    // stores a specific label
    char *label;
    
    // used to iterate through the url
    int url_index = 0;

    // reset the packet index back to the end of the header
    packet_index = HEADER_OFFSET;

    // populate the url
    while(1) {
        // update the label size
        label_size = packet->bytes[packet_index];

        // if the label has a size of zero it must be the end of the url
        if (label_size == 0) {
            // add a null byte and exit the while loop
            url[url_index++] = NULL_BYTE;
            break;
        }

        // if it is not the first entry add a dot separator
        if (packet_index != HEADER_OFFSET) {
            url[url_index++] = DOT_SEPARATOR;
        }

        // print each character in the label
        for (int i = 1; i <= label_size; i++ ) {

            // todo clean this up
            char next_char = packet->bytes[packet_index + i];
            url[url_index++] = next_char;
        }

        // increase the packet index by the label size plus one
        packet_index = packet_index + label_size + 1;
    }

    // todo

    // to store the type
    byte_t first_type_byte, second_type_byte;
    int type;

    // increment the next byte
    packet_index++;

    // get the next two bytes, increment afterwards
    first_type_byte = packet->bytes[packet_index++];
    second_type_byte = packet->bytes[packet_index++];

    type = two_bytes_to_integer(first_type_byte, second_type_byte);

    // display the formatted time, this is a test
    char *timestamp = get_timestamp();

    printf("%s ", timestamp);

    if (type != AAAA_TYPE) {
        printf("unimplemented request\n");
    }
    else {
        if (is_response) {
            printf("%s is at\n", url);
        }
    }

    // done with the timestamp, the packet and the label
    free_timestamp(timestamp);
    free_byte_array(packet);
    free(url);
}

// --- Helper Function Implementations ---