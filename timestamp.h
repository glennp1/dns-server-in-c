//
// Created by Glenn Phillips on 17/5/21.
//

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

// --- System Libraries ---

// --- Project Libraries ---

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// Returns a string representation of the current time
char *get_timestamp();

// free the specified timestamp and all of its associated memory
void free_timestamp(char *timestamp);

#endif //TIMESTAMP_H
