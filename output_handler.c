//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  output the log entry to the file ./dns_svr.log

// --- System Libraries ---
#include <stdio.h> // for output
#include <stdbool.h> // for booleans

#include <arpa/inet.h> // for inet_ntop and inet_pton

#include <string.h> // for strcat

// todo remove
#include <stdlib.h>

// --- Project Libraries ---
#include "output_handler.h"
#include "byte_converter.h"
#include "timestamp.h"

// --- Constant Definitions ---

// number of bytes reserved for the header
#define HEADER_OFFSET 12
#define AAAA_QTYPE 28 // for qtype
#define IN_QCLASS 1 // for qclass
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
    char url[url_size * sizeof(char)];

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
            // add a null byte to the end of the url
            url[url_index] = NULL_BYTE;

            // increment the packet index to the next byte
            packet_index++;

            // exit the while loop
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

    // to store the qtype
    byte_t first_qtype_byte, second_qtype_byte;
    int qtype;

    // get the next two bytes, increment afterwards
    first_qtype_byte = packet->bytes[packet_index++];
    second_qtype_byte = packet->bytes[packet_index++];

    qtype = two_bytes_to_integer(first_qtype_byte, second_qtype_byte);

    // todo clean this up
    // skip over the following 12 bytes to get to the rd length
    packet_index += 12;

    // store the next two bytes as the rd length
    byte_t first_rd_length_byte, second_rd_length_byte;
    int rd_length;

    first_rd_length_byte = packet->bytes[packet_index++];
    second_rd_length_byte = packet->bytes[packet_index++];

    rd_length = two_bytes_to_integer(first_rd_length_byte, second_rd_length_byte);

    // todo src must be network address structure

    byte_t ip_address_bytes[sizeof(struct in6_addr)];

    // store each of the bytes in the ip address
    for (int i = 0; i < rd_length; i++) {
        ip_address_bytes[i] = packet->bytes[packet_index++];
    }

    // todo dst must be the string
    char ip_address_string[INET6_ADDRSTRLEN];

    // todo figure this out
    // rd data
    inet_ntop(AF_INET6, &ip_address_bytes, ip_address_string, INET6_ADDRSTRLEN);

    char *timestamp = get_timestamp();

    FILE *file;

    file = fopen("dns_svr.log", "a");

    fprintf(file, "%s ", timestamp);

    if (qtype != AAAA_QTYPE) {
        fprintf(file, "unimplemented request\n");
    }
    else {
        if (is_response) {
            fprintf(file, "%s is at %s\n", url, ip_address_string);
        }
    }

    // todo not sure if this is necessary
    // to ensure that log updates are timely, as specified in the spec
    fflush(file);

    // close the file
    fclose(file);



    // done with the timestamp amd the packet
    free_timestamp(timestamp);
    free_byte_array(packet);
}

// --- Helper Function Implementations ---