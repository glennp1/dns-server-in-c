//
// Created by Glenn Phillips on 17/5/21.
//

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

// --- System Libraries ---

// --- Project Libraries ---

#include "byte_converter.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---
// parses the input from standard input and returns the packet read in
byte_array_t *parse_input(int num_args, char **args);

#endif //INPUT_HANDLER_H
