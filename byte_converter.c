//
// Created by Glenn Phillips on 17/5/21.
//

// --- System Libraries ---

#include <stdlib.h> // for malloc

// --- Project Libraries ---
#include "byte_converter.h"

// --- Constant Definitions ---

#define EIGHT_BIT_SHIFT 8

#define SINGLE_BIT 1
#define MAX_SHIFT_LEFT 7

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// Returns a pointer to a new array of bytes
byte_array_t *new_byte_array(int size) {

    // initialise the array
    byte_array_t *byte_array = malloc(sizeof(byte_array_t));

    // initialise the size and bytes
    byte_array->size = size;
    byte_array->bytes = malloc(size * sizeof(byte_t));

    return byte_array;
}

// Frees the specified array of bytes and all of its associated memory
void free_byte_array(byte_array_t *byte_array) {

    // size does not need to be freed

    // free the bytes
    free(byte_array->bytes);

    // free the byte array itself
    free(byte_array);
}

// Takes a byte array and an index as arguments
// Then takes the two bytes indicated by the index
// and returns them as a single integer
int two_bytes_to_integer(byte_t *bytes, int index) {

    // shift the first byte by eight bits left and merge it with the second byte
    return bytes[index] << EIGHT_BIT_SHIFT | bytes[index + 1];
 }

// Takes a single byte as an argument and returns it as an array of 8 bits
bit_t *one_byte_to_bits(byte_t input_byte) {
    byte_t bit_mask;
    bit_t *bits = malloc( BIT_ARRAY_SIZE * sizeof(bit_t));

    // starting from the first bit
    for (int i = 0; i < 8 ; i++) {
        // set the bit mask to correspond with the appropriate bit
        // this is done by shifting a single bit left from the
        // last column (0000 0001) to the appropriate column,
        // at most 7 spaces across (1000 0000)
        bit_mask = SINGLE_BIT << (MAX_SHIFT_LEFT-i);

        // check if any of the bits within the input byte match with the bit mask
        // if one bit does match then store a 1, otherwise store a 0
        bits[i] = (input_byte & bit_mask) ? 1 : 0;

        // eg:
        //
        // input_byte   1011 0111
        // bitmask      0010 0000
        //            = 0010 0000
        // resolves as true and therefore 1
        //
        // whereas:
        //
        // input_byte   1011 0111
        // bitmask      0000 1000
        //            = 0000 0000
        // resolves as false and therefore 0
    }
    return bits;
}

// Takes a single byte as an argument and changes the bit in the specified column
// to a one, returns the new byte
byte_t add_bit_to_bytes(byte_t byte, int col_from_right) {
    return SINGLE_BIT << (col_from_right) | byte;
}

// --- Helper Function Implementations ---
