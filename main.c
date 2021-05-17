//
// Created by Glenn Phillips on 10/5/21.
//

#include <stdio.h>

#include <unistd.h>

#define ONE_BYTE 1

typedef struct {
    // todo add the input arguments
} input_arguments_t;


int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    int input;
    int count = 1;

    while(read(STDIN_FILENO, &input, ONE_BYTE) > 0)
    {
        printf("%02x", input);
        
        if (count % 2 == 0) {
            printf(" ");
        }
        
        if (count % 16 == 0) {
            printf("\n");
        }
        
        count++;
    }

    printf("\n");

    return 0;

}


// read in a packet from std in

// input_arguments_t handle_input(int num_arguments, char *arguments[]) {
//
//
// }


// parse the packet



// output the log entry to the file ./dns_svr.log


