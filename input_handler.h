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

// Reads in the specified number of bytes from the specified input file
// and stores them within a newly created array of bytes
// then returns a pointer to this array
byte_t *read_in_bytes(int input_file, int num_bytes);

#endif //INPUT_HANDLER_H
