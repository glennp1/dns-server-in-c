//
// Created by Glenn Phillips on 10/5/21.
//

// --- System Libraries ---

// --- Project Libraries ---
#include "byte_converter.h"
#include "input_handler.h"
#include "output_handler.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

int main(int argc, char *argv[]) {

    byte_array_t *packet = parse_input(argc, argv);

    display_output(packet);

    return 0;
}

// --- Helper Function Implementations ---
