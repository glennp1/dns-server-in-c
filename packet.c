//
// Created by Glenn Phillips on 18/5/21.
//

// --- System Libraries ---
#include <stdlib.h> // for malloc
#include <arpa/inet.h> // for inet_ntop and inet_pton


// todo remove
#include <stdio.h>

// --- Project Libraries ---
#include "packet.h"

// --- Constant Definitions ---

#define IS_RES_BYTE_INDEX 2
#define IS_RES_BIT_INDEX 0

#define HEADER_OFFSET 12
#define ONE_BYTE_OFFSET 1

#define FIRST_INDEX 0
#define NULL_BYTE '\0'
#define DOT_SEPARATOR '.'

#define RDLENGTH_OFFSET_FROM_URL_END 14
#define IP_ADDR_OFFSET_FROM_URL_END 16

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// extracts if the packet is a response or a request
void extract_is_response(packet_t *packet);

// extracts the size of the url within the packet
void extract_url_size(packet_t *packet);

// extracts the url stored within the packet
void extract_url(packet_t *packet);

// extracts the type of the packet, must be AAAA
void extract_qtype(packet_t *packet);

// extracts the length of the ip address
void extract_rdlength(packet_t *packet);

// extracts the ip address stored within the packet
void extract_ip_address(packet_t *packet);

// --- Function Implementations ---

// todo new packet
packet_t *new_packet(byte_t *bytes, int length) {

    // initialise the packet
    packet_t *packet = malloc(sizeof(packet_t));

    // initialise the bytes and length
    packet->bytes = bytes;
    packet->length = length;

    // initialise the other parameters
    extract_is_response(packet);
    extract_url_size(packet);
    extract_url(packet);
    extract_qtype(packet);
    extract_rdlength(packet);
    extract_ip_address(packet);

    return packet;
}

// todo free packet
void free_packet(packet_t *packet) {

    // free the bytes
    free(packet->bytes);

    // length does not need to be freed

    // todo free other parameters
    free(packet->url);
    free(packet->ip_address);

    // free the packet itself
    free(packet);
}

// --- Helper Function Implementations ---

// extracts if the packet is a response or a request
void extract_is_response(packet_t *packet) {
    // convert the 3rd byte in the packet to bits
    bit_t *bits = one_byte_to_bits(packet->bytes[IS_RES_BYTE_INDEX]);

    // store if response or request
    packet->is_response = bits[IS_RES_BIT_INDEX];

    // todo change this to separate function
    // done with bits
    free(bits);
}

// extracts the size of the url within the packet
void extract_url_size(packet_t *packet) {

    // stores the size of a specific label
    int label_size;

    // used to iterate through the current packet
    // set to the end of the header
    int packet_index = HEADER_OFFSET;

    // add each label size to the url size until the end of the url is reached
    while(1) {
        // update the url size
        label_size = packet->bytes[packet_index];
        packet->url_size += label_size;

        // if the label has a size of zero it must be the end of the url
        if (label_size == 0) {
            break;
        }

        // leave space for the dot separators and the null byte at the end
        packet->url_size++;

        // increase the packet index by the label size plus the one byte offset
        packet_index = packet_index + label_size + ONE_BYTE_OFFSET;
    }
}

// extracts the url stored within the packet
void extract_url(packet_t *packet) {

    // allocate the correct amount of memory to the url
    packet->url = malloc(packet->url_size * sizeof(char));

    // stores a specific label and its size
    char *label;
    int label_size;

    // used to iterate through the url
    int url_index = FIRST_INDEX;

    // used to iterate through the current packet
    // set to the end of the header
    int packet_index = HEADER_OFFSET;

    // add each label to the url until the end of the url is reached
    while(1) {
        // update the label size
        label_size = packet->bytes[packet_index];

        // if the label has a size of zero it must be the end of the url
        if (label_size == 0) {
            // add a null byte to the end of the url
            packet->url[url_index] = NULL_BYTE;

            // exit the while loop
            break;
        }

        // if it is not the first entry add a dot separator
        if (packet_index != HEADER_OFFSET) {
            packet->url[url_index++] = DOT_SEPARATOR;
        }

        // add each character in the label to the url
        for (int i = 1; i <= label_size; i++ ) {
            packet->url[url_index++] = packet->bytes[packet_index + i];
        }

        // increase the packet index by the label size plus the one byte offset
        packet_index = packet_index + label_size + ONE_BYTE_OFFSET;
    }
}

// extracts the type of the packet, must be AAAA
void extract_qtype(packet_t *packet) {

    // used to iterate through the current packet
    // set to the end of the header plus the size of the url
    int packet_index = HEADER_OFFSET + packet->url_size + ONE_BYTE_OFFSET;

    // store the two bytes at the packet index as the qtype
    packet->qtype = two_bytes_to_integer(packet->bytes, packet_index);
}

// extracts the length of the ip address
void extract_rdlength(packet_t *packet) {

    // todo fix
    // if the packet is a request then this cannot be extracted
    if (!packet->is_response) {
        return;
    }

    // used to iterate through the current packet
    // set to the end of the header plus the size of the url
    // plus the rdlength offset
    int packet_index = HEADER_OFFSET + packet->url_size
            + RDLENGTH_OFFSET_FROM_URL_END + ONE_BYTE_OFFSET;

    // store the two bytes at the packet index as the rdlength
    packet->rdlength = two_bytes_to_integer(packet->bytes, packet_index);
}

// extracts the ip address stored within the packet
void extract_ip_address(packet_t *packet) {

    // todo fix
    // if the packet is a request then this cannot be extracted
    if (!packet->is_response) {
        return;
    }

    // used to iterate through the current packet
    // set to the end of the header plus the size of the url
    // plus the ip address offset
    int packet_index = HEADER_OFFSET + packet->url_size
                       + IP_ADDR_OFFSET_FROM_URL_END + ONE_BYTE_OFFSET;

    // ip address bytes must store the network address structure
    byte_t ip_address_bytes[sizeof(struct in6_addr)];

    // store each of the bytes in the ip address
    for (int i = 0; i < packet->rdlength; i++) {
        ip_address_bytes[i] = packet->bytes[packet_index++];
    }

    // ip address string must be the of an approriate size
    packet->ip_address = malloc(INET6_ADDRSTRLEN * sizeof(char));

    // todo figure this out
    // rd data
    inet_ntop(AF_INET6, &ip_address_bytes, packet->ip_address, INET6_ADDRSTRLEN);

}

