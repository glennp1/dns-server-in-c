//
// Created by Glenn Phillips on 17/5/21.
//

#ifndef BYTE_CONVERTER_H
#define BYTE_CONVERTER_H

// --- System Libraries ---

// --- Project Libraries ---

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// Reads in two bytes from standard input and returns them as a single integer
int two_bytes_to_integer(unsigned char first_byte, unsigned char second_byte);

// Reads in one bytes from standard input and returns them as an array of 8 bits
unsigned char *one_byte_to_bits(unsigned char first_byte);

#endif //BYTE_CONVERTER_H
