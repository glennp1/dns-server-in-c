//
// Created by Glenn Phillips on 17/5/21.
//

// --- System Libraries ---
#include <time.h> // for strftime, time etc.
#include <stdlib.h> // for malloc

// todo remove
#include <stdio.h>

// --- Project Libraries ---
#include "timestamp.h"

// --- Constant Definitions ---
#define MIN_TIME_SIZE 10
#define TIME_FORMAT "%FT%T%z"

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// Returns a string representation of the current time
char *get_timestamp() {

    // to store the size of the formatted timestamp, start this small
    int time_size = MIN_TIME_SIZE;

    // to store time, local time, and the formatted timestamp
    time_t raw_time;
    struct tm *local_time;
    char *timestamp = malloc(time_size);

    // store the current time in raw time
    time(&raw_time);

    // convert it the correct timezone
    local_time = localtime(&raw_time);

    // while formatting the time is unsuccessful
    while (strftime(timestamp, time_size, TIME_FORMAT, local_time) == 0) {

        // free the timestamp
        free_timestamp(timestamp);

        // double the size of the timestamp
        time_size *= 2;

        // allocate memory to the time stamp and try again
        timestamp = malloc(time_size);

        // todo remove
        // printf("new size: %d\n", time_size);
    }

    // return the formatted timestamp
    return timestamp;
}

// free the specified timestamp and all of its associated memory
void free_timestamp(char *timestamp) {
    free(timestamp);
}

// --- Helper Function Implementations ---
