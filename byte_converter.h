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

// represents an array of bytes
typedef struct byte_array_s byte_array_t;

struct byte_array_s {
    int size;
    byte_t *bytes;
};

// --- Function Prototypes ---

// Returns a pointer to a new array of bytes
byte_array_t *new_byte_array();

// Frees the specified array of bytes and all of its associated memory
void free_byte_array(byte_array_t *byte_array);

// Takes two bytes as arguments and returns them as a single integer
int two_bytes_to_integer(byte_t first, byte_t second);

// Takes a single byte as an argument and returns it as an array of 8 bits
bit_t *one_byte_to_bits(byte_t input_byte);

#endif //BYTE_CONVERTER_H
