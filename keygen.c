// Author: Nicholas Graves
// Class: CS 344
// Assignment: Assignment 5 - One Time Pads
// Due Date: 6/14/23
// File: keygen.c - This program creates a key file of specified length

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// sets up valid characters constant
const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";


int main(int argc, char *argv[]) 
{
    // seed the random number generator with the current time
    srand(time(NULL));
    
    // check if the number of command line arguments is 2
    if (argc != 2) 
    {
        fprintf(stderr, "usage: ./keygen length\n");
        exit(1);
    }

    // convert inputed length to int
    int length = atoi(argv[1]);

    // generate a random key of the specified length
    for (int i = 0; i < length; i++) 
    {
        int randNum = rand() % 27;
        char output = (randNum < 26) ? alphabet[randNum] : ' ';
        printf("%c", output);
    }
    
    // print a newline character and flush the output buffer
    printf("\n");
    fflush(stdout);
    
    return 0;
}

