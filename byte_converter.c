//
// Created by Glenn Phillips on 17/5/21.
//

// --- System Libraries ---

#include <stdlib.h> // for malloc

// --- Project Libraries ---
#include "byte_converter.h"

// --- Constant Definitions ---

#define EIGHT_BIT_SHIFT 8

#define ONE_BIT 1
#define MAX_SHIFT_LEFT 7

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

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
        bit_mask = ONE_BIT << (MAX_SHIFT_LEFT-i);

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

// Takes a single byte as an argument and changes the bit in the specified index
// to a one, returns the new byte
byte_t change_bit_in_byte_to_one(byte_t byte, int bit_index) {

    // calculate the column from the right
    int col_from_right = MAX_SHIFT_LEFT - bit_index;

    // to store the mask,
    // will look something like this "0000 0100"
    byte_t bit_mask = ONE_BIT << (col_from_right);

    // change the bit to a one
    return bit_mask | byte;
}

// Takes a single byte as an argument and changes the bit in the specified index
// to a zero, returns the new byte
byte_t change_bit_in_byte_to_zero(byte_t byte, int bit_index) {

    // calculate the column from the right
    int col_from_right = MAX_SHIFT_LEFT - bit_index;

    // to store the mask, note that it is inverted
    // will look something like this "1111 1011"
    byte_t bit_mask = ~(ONE_BIT << (col_from_right));

    // change the bit to a zero
    return bit_mask & byte;
}

// --- Helper Function Implementations ---
