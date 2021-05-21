//
// Created by Glenn Phillips on 17/5/21.
//

// --- System Libraries ---
#include <unistd.h> // for read
#include <stdlib.h> // for malloc
#include <stdio.h>

// --- Project Libraries ---
#include "input_handler.h"

// --- Constant Definitions ---

#define ONE_BYTE 1

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// Reads from specified input file one byte at a time,
// does so until the num bytes has been read,
// the bytes are stored in the array of bytes specified
void read_byte_by_byte(int input_file, byte_t *bytes, int num_bytes) {

    // to keep track of how many bytes have been read successfully
    int total_bytes_read = 0;
    int num_bytes_read;

    // continue to read until the total bytes read equals the number of bytes
    while(total_bytes_read != num_bytes) {

        // read from the input file and store it,
        // updating num_bytes_read in the process
        num_bytes_read = read(input_file, (bytes + total_bytes_read), ONE_BYTE);

        // then increase the total bytes read
        total_bytes_read += num_bytes_read;
    }
}

// --- Helper Function Implementations ---

