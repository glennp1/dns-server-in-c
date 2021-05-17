//
// Created by Glenn Phillips on 17/5/21.
//

#ifndef BYTE_CONVERTER_H
#define BYTE_CONVERTER_H

// --- System Libraries ---

// --- Project Libraries ---

// --- Constant Definitions ---

// --- Type Definitions ---

// represents one byte
typedef unsigned char byte_t;

// represents one bit
typedef unsigned char bit_t;

// --- Function Prototypes ---

// Reads in two bytes from standard input and returns them as a single integer
int two_bytes_to_integer(byte_t first, byte_t second);

// Reads in one byte from standard input and returns it as an array of 8 bits
bit_t *one_byte_to_bits(byte_t input_byte);

#endif //BYTE_CONVERTER_H
