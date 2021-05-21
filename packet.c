//
// Created by Glenn Phillips on 18/5/21.
//

// --- System Libraries ---
#include <stdlib.h> // for malloc
#include <arpa/inet.h> // for inet_ntop and inet_pton
#include <unistd.h> // for read

// todo remove
#include <stdio.h>

// --- Project Libraries ---
#include "packet.h"
#include "input_handler.h"

// --- Constant Definitions ---

// For unimplemented request
#define PACKET_RCODE_INDEX 5
#define FIFTH_BIT 4
#define SIXTH_BIT 5
#define SEVENTH_BIT 6
#define EIGHT_BIT 7


// The number of bytes that stores the packet length
#define PACKET_LENGTH_SIZE 2

#define IS_RES_BYTE_INDEX 4
#define IS_RES_BIT_INDEX 0

// number of bytes reserved for the header
#define HEADER_OFFSET 14
#define ONE_BYTE_OFFSET 1

#define FIRST_INDEX 0
#define NULL_BYTE '\0'
#define DOT_SEPARATOR '.'

#define AAAA_TYPE 28 // for qtype and first atype
#define QTYPE_OFFSET_FROM_URL_END 1
#define FIRST_ATYPE_OFFSET_FROM_URL_END 7

#define RDLENGTH_OFFSET_FROM_URL_END 15
#define IP_ADDR_OFFSET_FROM_URL_END 17

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// extracts the length of the packet and uses this to store the bytes
void extract_length_and_bytes(packet_t *packet, int input_file);

// extracts if the packet is a response or a request
void extract_is_response(packet_t *packet);

// extracts the size of the url within the packet
void extract_url_size(packet_t *packet);

// extracts the url stored within the packet
void extract_url(packet_t *packet);

// extracts the if the qtype of the packet is AAAA
void extract_qtype_is_aaaa(packet_t *packet);

// extracts the if the first atype of the packet is AAAA
void extract_first_atype_is_aaaa(packet_t *packet);

// extracts the length of the ip address
void extract_rdlength(packet_t *packet);

// extracts the ip address stored within the packet
void extract_ip_address(packet_t *packet);

// prints the bytes in the packet, this is used for debugging
void print_packet_bytes(packet_t *packet);

// --- Function Implementations ---

// creates and returns a new packet based on the specified input
packet_t *new_packet(int input_file) {

    // initialise the packet
    packet_t *packet = malloc(sizeof(packet_t));

    extract_length_and_bytes(packet, input_file);

    // initialise the other parameters
    extract_is_response(packet);
    extract_url_size(packet);
    extract_url(packet);
    extract_qtype_is_aaaa(packet);

    //  if the packet is a response that has the correct question type
    if (packet->is_response && packet->qtype_is_aaaa) {

        // initialise whether the packet answer type is aaaa
        extract_first_atype_is_aaaa(packet);

        // if the packet has the correct answer type
        if (packet->first_atype_is_aaaa) {

            // initialise the length of the ip and and ip itself
            extract_rdlength(packet);
            extract_ip_address(packet);
        }
    }

    return packet;
}

// changes the packet rcode to 4 - unimplemented request
void change_packet_rcode(packet_t *packet) {

    // todo remove
    printf("%02x\n", packet->bytes[PACKET_RCODE_INDEX]);

    // we want the rcode to be 4
    // the rcode is the last four bits of the byte
    // in binary that is "~~~~0100"
    // so we need to update the 5th, 6th, 7th and 8th bit

    // change 5th to 0
    packet->bytes[PACKET_RCODE_INDEX] = change_bit_in_byte_to_zero(
            packet->bytes[PACKET_RCODE_INDEX], FIFTH_BIT);

    // change 6th to 1
    packet->bytes[PACKET_RCODE_INDEX] = change_bit_in_byte_to_one(
            packet->bytes[PACKET_RCODE_INDEX], SIXTH_BIT);

    // change 7th to 0
    packet->bytes[PACKET_RCODE_INDEX] = change_bit_in_byte_to_zero(
            packet->bytes[PACKET_RCODE_INDEX], SEVENTH_BIT);


    // change 8th to 0
    packet->bytes[PACKET_RCODE_INDEX] = change_bit_in_byte_to_zero(
            packet->bytes[PACKET_RCODE_INDEX], EIGHT_BIT);


    printf("%02x\n", packet->bytes[PACKET_RCODE_INDEX]);
}

// frees the specified packet and all of its associated memory
void free_packet(packet_t *packet) {

    // free everything stored within the packet
    free(packet->bytes);
    free(packet->url);

    // only need to free the ip address if the packet has a response
    if (packet->is_response) {
        free(packet->ip_address);
    }

    // free the packet itself
    free(packet);
}

// --- Helper Function Implementations ---

// extracts the length of the packet and uses this to store the bytes
void extract_length_and_bytes(packet_t *packet, int input_file) {
    // to store the number of bytes that haven't been read yet
    int remaining_packet_length;

    // allocate memory to store the bytes within the packet
    packet->bytes = malloc(PACKET_LENGTH_SIZE * sizeof(byte_t));

    // read in the bytes that store the length of the remaining packet
    read(input_file, packet->bytes, PACKET_LENGTH_SIZE);

    // convert these two bytes into an integer to store the packet length
    remaining_packet_length = two_bytes_to_integer(packet->bytes, 0);

    // calculate and store the length of the entire packet
    packet->length = PACKET_LENGTH_SIZE + remaining_packet_length;

    // reallocate the memory to store all of the bytes within the packet
    packet->bytes = realloc(packet->bytes, packet->length * sizeof(byte_t));

    // read in the remaining packet, starting from the end
    // of the packet length bytes
    read(input_file, packet->bytes + PACKET_LENGTH_SIZE, remaining_packet_length);

    // todo remove
    printf("packet length: %d\n", packet->length);
    print_packet_bytes(packet);
}

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

    // set the url size to zero to begin with
    packet->url_size = 0;

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

    // stores the size of a specific label
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

// extracts the if the qtype of the packet is AAAA
void extract_qtype_is_aaaa(packet_t *packet) {

    // used to iterate through the current packet
    int packet_index = HEADER_OFFSET + packet->url_size
            + QTYPE_OFFSET_FROM_URL_END;

    // store the two bytes at the packet index as the qtype
    int qtype = two_bytes_to_integer(packet->bytes, packet_index);

    // store if the qtype matches AAAA
    packet->qtype_is_aaaa = (qtype == AAAA_TYPE);
}

// extracts the if the first atype of the packet is AAAA
void extract_first_atype_is_aaaa(packet_t *packet) {

    // used to iterate through the current packet
    int packet_index = HEADER_OFFSET + packet->url_size
            + FIRST_ATYPE_OFFSET_FROM_URL_END;

    // store the two bytes at the packet index as the first atype
    int first_atype = two_bytes_to_integer(packet->bytes, packet_index);

    // store if the first atype matches AAAA
    packet->first_atype_is_aaaa = (first_atype == AAAA_TYPE);
}

// extracts the length of the ip address
void extract_rdlength(packet_t *packet) {

    // used to iterate through the current packet
    int packet_index = HEADER_OFFSET + packet->url_size
            + RDLENGTH_OFFSET_FROM_URL_END;

    // store the two bytes at the packet index as the rdlength
    packet->rdlength = two_bytes_to_integer(packet->bytes, packet_index);
}

// extracts the ip address stored within the packet
void extract_ip_address(packet_t *packet) {

    // used to iterate through the current packet
    int packet_index = HEADER_OFFSET + packet->url_size
                       + IP_ADDR_OFFSET_FROM_URL_END;

    // ip address bytes must store the network address structure
    byte_t ip_address_bytes[sizeof(struct in6_addr)];

    // store each of the bytes in the ip address
    for (int i = 0; i < packet->rdlength; i++) {
        ip_address_bytes[i] = packet->bytes[packet_index++];
    }

    // ip address string must be the of an appropriate size
    packet->ip_address = malloc(INET6_ADDRSTRLEN * sizeof(char));

    // todo not sure if this needs to handle ipv4 addresses as well?
    // convert the ip address in bytes into a string and store it in
    // packet->ip_address
    inet_ntop(AF_INET6, &ip_address_bytes, packet->ip_address, INET6_ADDRSTRLEN);
}

// prints the bytes in the packet, this is used for debugging
void print_packet_bytes(packet_t *packet) {
    for (int i = 1; i <= packet->length; i++) {
        printf("%02x", packet->bytes[i-1]);

        if (i % 2 == 0) {
            printf(" ");
        }

        if (i % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
