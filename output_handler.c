//
// Created by Glenn Phillips on 17/5/21.
//
// todo
//  output the log entry to the file ./dns_svr.log

// --- System Libraries ---
#include <stdio.h> // for output

// --- Project Libraries ---
#include "output_handler.h"
#include "timestamp.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

void display_output() {
    // display the formatted time, this is a test
    char *timestamp = get_timestamp();

    printf("%s\n", timestamp);

    free_timestamp(timestamp);
}

// --- Helper Function Implementations ---