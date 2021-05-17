//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  read in a packet from std in
//  parse the packet

// --- System Libraries ---
#include <unistd.h> // for read

// todo remove
#include <stdio.h>

// --- Project Libraries ---
#include "input_handler.h"
#include "byte_converter.h"


// --- Constant Definitions ---

#define ONE_BYTE 1

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// parses the input from standard input
void parse_input(int num_args, char **args) {

    // store the input arguments
    for (int i = 1; i < num_args; i++) {
        printf("%s\n", args[i]);
    }

    // to store two separate bytes
    byte_t first_byte, second_byte;

    // read in the first two bytes from the file in standard input and print them in hexadecimal
    read(STDIN_FILENO, &first_byte, ONE_BYTE);
    read(STDIN_FILENO, &second_byte, ONE_BYTE);
    printf("%02x ", first_byte);
    printf("%02x\n", second_byte);

    // print the first two bytes converted to an integer
    printf("%d\n", two_bytes_to_integer(first_byte, second_byte));

    // read in the next two bytes from the file in standard input and print them in hexadecimal
    read(STDIN_FILENO, &first_byte, ONE_BYTE);
    read(STDIN_FILENO, &second_byte, ONE_BYTE);
    printf("%02x ", first_byte);
    printf("%02x\n", second_byte);

    // read in the next byte and print it in hexadecimal
    read(STDIN_FILENO, &first_byte, ONE_BYTE);
    printf("%02x\n", first_byte);

    // leave a line blank
    printf("\n");

    // convert this last byte into bits
    bit_t *bits = one_byte_to_bits(first_byte);

    // print each bit
    for (int i = 0; i < 8; i++) {
        printf("%d\n", *(bits+i));
    }
}

// --- Helper Function Implementations ---