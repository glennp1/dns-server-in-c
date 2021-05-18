//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  read in a packet from std in
//  parse the packet

// --- System Libraries ---
#include <unistd.h> // for read
#include <stdlib.h> // for malloc

// todo remove
#include <stdio.h>

// --- Project Libraries ---
#include "input_handler.h"

// --- Constant Definitions ---

#define PACKET_LENGTH_SIZE 2

#define ONE_BYTE 1

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// Reads in the specified number of bytes from standard input and
// stores them within a newly created array of bytes
// then returns a pointer to this array
byte_t *read_in_bytes(int num_bytes);

// --- Function Implementations ---

// parses the input from standard input and returns the packet read in
packet_t *parse_input(int num_args, char **args) {

//    // todo store the input arguments?
//    for (int i = 1; i < num_args; i++) {
//        printf("%s\n", args[i]);
//    }

    // to store the bytes containing the length of the packet
    byte_t *packet_length_bytes;

    // read in the packet length bytes
    packet_length_bytes = read_in_bytes(PACKET_LENGTH_SIZE);

    // convert these two bytes into an integer
    int packet_length = two_bytes_to_integer(packet_length_bytes, 0);

    // we are done with this
    free(packet_length_bytes);

    // to store the bytes within the actual packet
    byte_t *packet_bytes;

    // read in the packet bytes
    packet_bytes = read_in_bytes(packet_length);

    // create a new packet
    packet_t *packet = new_packet(packet_bytes, packet_length);

    return packet;
}

// --- Helper Function Implementations ---

// Reads in the specified number of bytes from standard input and
// stores them within a newly created array of bytes
// then returns a pointer to this array
byte_t *read_in_bytes(int num_bytes) {

    // to store the array of bytes
    byte_t *bytes = malloc(num_bytes * sizeof(byte_t));

    // read in the bytes and store them in the bytes array
    for (int i = 0; i < num_bytes; i++) {
        read(STDIN_FILENO, (bytes + i), ONE_BYTE);
    }

    return bytes;
}