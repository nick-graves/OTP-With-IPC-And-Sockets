// Author: Nicholas Graves
// Class: CS 344
// Assignment: Assignment 5 - One Time Pads
// Due Date: 6/14/23
// File: dec_server.c - This program is the decryption server and will run in the background as a daemon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>

// function headers
void reportError(const char *msg);
void setupServerAddress(struct sockaddr_in *address, int portNumber);
void processClientRequest(int connectionSocket);
void extractKeyAndText(const char *buffer, char *key, char *text);
void decryptText(char *text, const char *key);


// print error function
void reportError(const char *msg)
{
    perror(msg);
    exit(1);
}


// set up the address struct for the server socket
void setupServerAddress(struct sockaddr_in *address, int portNumber)
{
    // clear out the address struct
    memset((char *)address, '\0', sizeof(*address));

    // the address should be network capable
    address->sin_family = AF_INET;

    // store the port number
    address->sin_port = htons(portNumber);

    // allow a client at any address to connect to this server
    address->sin_addr.s_addr = INADDR_ANY;
}


// extracts the key and text from a buffer
void extractKeyAndText(const char *buffer, char *key, char *text)
{
    // find the index of the '&' character in the buffer
    int findMiddle = 0;
    while (buffer[findMiddle] != '&')
    {
        findMiddle++;
    }

    // extract key
    strncpy(key, buffer, findMiddle);
    key[findMiddle] = '\0';

    // extract text
    strcpy(text, buffer + findMiddle + 1);
}


// decrypts a text using a key
void decryptText(char *text, const char *key)
{
    // get the length of the text and key
    int textLength = strlen(text);
    int keyLength = strlen(key);

    // decrypt each character in the text using the corresponding character in the key
    for (int i = 0; i < textLength; i++)
    {
        // decrypt the character using the key
        int decryptedText = text[i] - 65;
        int decryptedKey = key[i % keyLength] - 65;

        // handle wraparound for negative values
        if (decryptedText < 0)
        {
            decryptedText = 26;
        }
        if (decryptedKey < 0)
        {
            decryptedKey = 26;
        }

        // calculate the decrypted value
        int decryptedValue = (decryptedText - decryptedKey + 27) % 27;

        // handle wraparound for value than 26
        if (decryptedValue == 26)
        {
            text[i] = 32;
        }
        else
        {
            text[i] = decryptedValue + 65;
        }

    }
}

// processes a client request by receiving the message from the client, extracting the key and text, decrypting the text using the key, and sending the 
// decrypted text back to the client.
void processClientRequest(int connectionSocket)
{
    // initialize variables
    int charsRead;
    char buffer[100000];

    // get the message from the client and display it
    memset(buffer, '\0', sizeof(buffer));

    // read the client's message from the socket
    int total = 0;
    int flag = 1;

    // loop until all data is read from the socket
    while (flag)
    {
        // reads data
        charsRead = recv(connectionSocket, &buffer[total], sizeof(buffer) - total - 1, 0);
        fflush(stdout);

        // error reading from socket
        if (charsRead < 0)
        {
            reportError("ERROR reading from socket");
        }

        // tracks total chars read
        total += charsRead;

        // checks if there is no more data to be read
        if (charsRead == 0 || buffer[total - 1] == '>')
        {
            flag = 0;
            break;
        }
    }


    // extract key and text from the received buffer
    char key[total];
    char text[total];
    extractKeyAndText(buffer, key, text);

    text[total - 1] = '\0';

    // decrypt the text using the key
    decryptText(text, key);


    // send the decrypted text back to the client
    charsRead = send(connectionSocket, text, strlen(text), 0);
    if (charsRead < 0)
    {
        reportError("ERROR writing to socket");
    }

    // close the connection socket for this client
    close(connectionSocket);
}

// main function that calls all previously defined functions
int main(int argc, char *argv[])
{
    int connectionSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t sizeOfClientInfo = sizeof(clientAddress);

    // check arguments
    if (argc < 2)
    {
        fprintf(stderr, "USAGE: %s port\n", argv[0]);
        exit(1);
    }

    // create the socket that will listen for connections
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0)
    {
        reportError("ERROR opening socket");
    }

    // set up the address struct for the server socket
    setupServerAddress(&serverAddress, atoi(argv[1]));

    // associate the socket to the port
    if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        reportError("ERROR on binding");
    }

    // start listening for connections allow 5
    listen(listenSocket, 5);

    // acccept a connection blocking if one is not available until one connects
    while (1)
    {
        // accept the connection request which creates a connection socket
        connectionSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);
        if (connectionSocket < 0)
        {
            reportError("ERROR on accept");
        }

        printf("SERVER: Connected to client running at host %d port %d\n", ntohs(clientAddress.sin_addr.s_addr), ntohs(clientAddress.sin_port));

        // process the client's request
        processClientRequest(connectionSocket);
    }

    // close the listening socket
    close(listenSocket);
    return 0;
}