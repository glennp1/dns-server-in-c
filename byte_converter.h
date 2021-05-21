//
// Created by Glenn Phillips on 17/5/21.
//

#ifndef BYTE_CONVERTER_H
#define BYTE_CONVERTER_H

// --- System Libraries ---

// --- Project Libraries ---

// --- Constant Definitions ---

#define BIT_ARRAY_SIZE 8

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

// Takes a byte array and an index as arguments
// Then takes the two bytes indicated by the index
// and returns them as a single integer
int two_bytes_to_integer(byte_t *bytes, int index);

// Takes a single byte as an argument and returns it as an array of 8 bits
bit_t *one_byte_to_bits(byte_t input_byte);

// Takes a single byte as an argument and changes the bit in the specified index
// to a one, returns the new byte
byte_t change_bit_in_byte_to_one(byte_t byte, int bit_index);

// Takes a single byte as an argument and changes the bit in the specified index
// to a zero, returns the new byte
byte_t change_bit_in_byte_to_zero(byte_t byte, int bit_index);

#endif //BYTE_CONVERTER_H
