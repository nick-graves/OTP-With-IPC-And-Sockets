// Author: Nicholas Graves
// Class: CS 344
// Assignment: Assignment 5 - One Time Pads
// Due Date: 6/14/23
// File: dec_client.c - This program connects to dec_server and asks it to perform a one-time pad style decryption

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>

// function definitions
void error(const char *msg);
void setupAddressStruct(struct sockaddr_in *address, int portNumber);
void sendData(int socketFD, char *data);
void processFile(int socketFD, char *fileName, char delimiter);

// print error function
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// set up the address struct
void setupAddressStruct(struct sockaddr_in *address, int portNumber)
{
    // clear out the address struct
    memset((char *)address, '\0', sizeof(*address));

    // the address should be network capable
    address->sin_family = AF_INET;
    // store the port number
    address->sin_port = htons(portNumber);

    // get the DNS entry for localhost
    struct hostent *hostInfo = gethostbyname("localhost");

    // error if not host
    if (hostInfo == NULL)
    {
        fprintf(stderr, "CLIENT: ERROR, no such host\n");
        exit(1);
    }
    // copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char *)&address->sin_addr.s_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
}

// sends data over a socket connection
void sendData(int socketFD, char *data)
{
    // send data over the socket connection
    int charsWritten = send(socketFD, data, strlen(data), 0);

    // check for error sending the data
    if (charsWritten < 0)
    {
        error("CLIENT: ERROR writing to socket");
    }

    // check if all data was written to socket
    if (charsWritten < strlen(data))
    {
        printf("CLIENT: WARNING: Not all data written to socket!\n");
    }
}

// processes a file and sends its contents over a socket connection
void processFile(int socketFD, char *fileName, char delimiter)
{
    char buffer[1024];

    // open the file for reading
    FILE *fp = fopen(fileName, "r");

    // error if file cant be opended
    if (fp == NULL)
    {
        printf("Failed to open %s\n", fileName);
        exit(1);
    }

    // initialize counter and character values
    int count = 0;
    char c;

    // loops untill file is empty
    while ((c = fgetc(fp)) != EOF && c != '\n')
    {
        // adds char and increments  count
        buffer[count] = c;
        count++;
        // if buffer is full send the data over the socket connection and clear buffer
        if (count == 999)
        {
            // clear buffer and send data
            buffer[count] = '\0';
            sendData(socketFD, buffer);
            memset(buffer, '\0', sizeof(buffer));
            count = 0;
        }
    }
    // add delimiter
    buffer[count] = delimiter;

    // send data
    sendData(socketFD, buffer);

    // clear buffer and close file
    memset(buffer, '\0', sizeof(buffer));
    fclose(fp);
}


// main function that calls all previously defined functions
int main(int argc, char *argv[])
{
    // initialize socket variables
    int socketFD, portNumber, charsRead;
    struct sockaddr_in serverAddress;

    // check arguments
    if (argc < 4)
    {
        fprintf(stderr, "USAGE: %s [plaintext] [key] [port]\n", argv[0]);
        exit(1);
    }

    // get the size of key and plaintext
    struct stat st;
    stat(argv[2], &st);
    int keySize = st.st_size;
    stat(argv[1], &st);
    int plainTextSize = st.st_size;
    fflush(stdout);

    // checks for size mismatch
    if (keySize < plainTextSize)
    {
        error("Error: Key is too short\n");
    }

    // creates buffer to hold file content
    char buffer[plainTextSize];

    // create a socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0)
    {
        error("CLIENT: ERROR opening socket");
    }

    // set up the server address struct
    setupAddressStruct(&serverAddress, atoi(argv[3]));

    // connect to server
    if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        error("CLIENT: ERROR connecting");
    }

    // process the key and plaintext file and send to server
    processFile(socketFD, argv[2], '&');
    processFile(socketFD, argv[1], '>');

    // recieve ciphertext from server
    memset(buffer, '\0', sizeof(buffer));
    charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);

    // error reading from socket
    if (charsRead < 0)
    {
        error("CLIENT: ERROR reading from socket");
    }

    // print to console
    printf("%s\n", buffer);

    // close the socket
    close(socketFD);
    return 0;
}