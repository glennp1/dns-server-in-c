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

#define ONE_BYTE 1

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// todo currently not used
// Reads in the specified number of bytes from the specified input file
// and stores them within a newly created array of bytes
// then returns a pointer to this array
byte_t *read_in_bytes(int input_file, int num_bytes) {

    // to store the array of bytes
    byte_t *bytes = malloc(num_bytes * sizeof(byte_t));

    // read in the bytes and store them in the bytes array
    for (int i = 0; i < num_bytes; i++) {
        read(input_file, (bytes + i), ONE_BYTE);
    }

    return bytes;
}

// --- Helper Function Implementations ---

