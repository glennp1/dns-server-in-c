//
// Created by Glenn Phillips on 17/5/21.
//

// --- System Libraries ---

// --- Project Libraries ---
#include "byte_converter.h"

// --- Constant Definitions ---

#define EIGHT_BIT_SHIFT 8

#define SINGLE_BIT 1
#define MAX_SHIFT_LEFT 7

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// todo potentially make bytes_to_integer function that handles all cases
// Reads in two bytes from standard input and returns them as a single integer
int two_bytes_to_integer(byte_t first, byte_t second) {
    // shift the first byte by eight bits left and merge it with the second byte
    return first << EIGHT_BIT_SHIFT | second;
}

// Reads in one byte from standard input and returns it as an array of 8 bits
bit_t *one_byte_to_bits(byte_t input_byte) {
    byte_t bit_mask;
    bit_t *bits;

    // starting from the first bit
    for (int i = 0; i < 8 ; i++) {
        // set the bit mask to correspond with the appropriate bit
        // this is done by shifting a single bit left from the
        // last column (0000 0001) to the appropriate column,
        // at most 7 spaces across (1000 0000)
        bit_mask = SINGLE_BIT << (MAX_SHIFT_LEFT-i);

        // check if any of the bits within the input byte match with the bit mask
        // if one bit does match then store a 1, otherwise store a 0
        *(bits+i) = (input_byte & bit_mask) ? 1 : 0;

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

// --- Helper Function Implementations ---
