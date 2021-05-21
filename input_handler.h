//
// Created by Glenn Phillips on 17/5/21.
//

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

// --- System Libraries ---

// --- Project Libraries ---
#include "packet.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// Reads from specified input file one byte at a time,
// does so until the num bytes has been read,
// the bytes are stored in the array of bytes specified
void read_byte_by_byte(int input_file, byte_t *bytes, int num_bytes);

#endif //INPUT_HANDLER_H
