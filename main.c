//
// Created by Glenn Phillips on 10/5/21.
//

// todo
//  read in a packet from std in
//  parse the packet
//  output the log entry to the file ./dns_svr.log

// --- System Libraries ---
#include <stdio.h> // for output
#include <unistd.h> // for read

// --- Project Libraries ---
#include "byte_converter.h"
#include "timestamp.h"

// --- Constant Definitions ---

#define ONE_BYTE 1

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

int main(int argc, char *argv[]) {

    // print the input arguments
    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    // to store two separate bytes
    unsigned char first_byte, second_byte;

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
    unsigned char *bits = one_byte_to_bits(first_byte);

    // print each bit
    for (int i = 0; i < 8; i++) {
        printf("%d\n", *(bits+i));
    }

    // display the formatted time, this is a test
    printf("%s\n", get_timestamp());

    return 0;
}

// --- Helper Function Implementations ---
