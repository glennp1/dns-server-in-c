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
byte_array_t *read_in_bytes(int num_bytes);

// --- Function Implementations ---

// parses the input from standard input and returns the packet read in
byte_array_t *parse_input(int num_args, char **args) {

    // todo store the input arguments?
    for (int i = 1; i < num_args; i++) {
        printf("%s\n", args[i]);
    }

    // to store the length of the packet
    byte_array_t *packet_length_in_bytes;
    int packet_length;

    // read in the packet length
    packet_length_in_bytes = read_in_bytes(PACKET_LENGTH_SIZE);

    // take the two bytes from the packet_length
    byte_t first_byte = packet_length_in_bytes->bytes[0];
    byte_t second_byte = packet_length_in_bytes->bytes[1];

    // convert these two bytes into an integer
    packet_length = two_bytes_to_integer(first_byte, second_byte);

    // we are done with the packet length in bytes
    free_byte_array(packet_length_in_bytes);

    // to store the packet itself as an array of bytes
    byte_array_t *packet;

    // read in the remainder of the packet
    packet = read_in_bytes(packet_length);

    return packet;
}

// --- Helper Function Implementations ---

// Reads in the specified number of bytes from standard input and
// stores them within a newly created array of bytes
// then returns a pointer to this array
byte_array_t *read_in_bytes(int num_bytes) {

    // Assign memory to a new array of bytes
    byte_array_t *byte_array = new_byte_array(num_bytes);

    // store the size of the bytes array
    byte_array->size = num_bytes;

    // read in the bytes and store them in the bytes array
    for (int i = 0; i < num_bytes; i++) {
        read(STDIN_FILENO, (byte_array->bytes + i), ONE_BYTE);
    }

    return byte_array;
}