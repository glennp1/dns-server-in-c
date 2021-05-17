//
// Created by Glenn Phillips on 17/5/21.
//

// --- System Libraries ---
#include <time.h> // for strftime, time etc.

// --- Project Libraries ---
#include "timestamp.h"

// --- Constant Definitions ---
#define TIME_SIZE 80
#define TIME_FORMAT "%FT%T%z"

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// --- Function Implementations ---

// Returns a string representation of the current time
char *get_timestamp() {
    // to store time, local time, and the formatted local time
    time_t raw_time;
    struct tm *local_time;
    char *formatted_time;

    // store the time in raw time
    time(&raw_time);

    // convert it the correct timezone
    local_time = localtime(&raw_time);

    // format the time as specified
    strftime(formatted_time, TIME_SIZE, TIME_FORMAT, local_time);

    // return the formatted time
    return formatted_time;
}

// --- Helper Function Implementations ---
