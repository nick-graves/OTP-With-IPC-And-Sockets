One-Time Pad Encryption and Decryption System
Introduction
This repository contains a set of C programs designed to implement a one-time pad-like encryption and decryption system using socket-based inter-process communication (IPC). The system includes servers (enc_server for encryption and dec_server for decryption) and corresponding clients (enc_client for encryption and dec_client for decryption). Additionally, a keygen utility is provided to generate random key files of specified lengths.

System Requirements
To run these programs successfully, ensure you have the following:

A Unix-like environment (Linux, macOS)
GCC (GNU Compiler Collection) installed
Basic understanding of socket programming in C
File Descriptions
enc_server: Listens on a specified port, receives plaintext and key from enc_client, performs encryption, and sends back ciphertext.
enc_client: Connects to enc_server, sends plaintext and key files, and receives ciphertext.
dec_server: Listens on a specified port, receives ciphertext and key from dec_client, performs decryption, and sends back plaintext.
dec_client: Connects to dec_server, sends ciphertext and key files, and receives plaintext.
keygen: Generates a key file of specified length containing random characters (A-Z and space).
Compilation
Compile all programs using the provided compileall script or Makefile:

bash
Copy code
$ ./compileall
Usage
enc_server:

bash
Copy code
$ ./enc_server <listening_port> &
enc_client:

bash
Copy code
$ ./enc_client <plaintext_file> <key_file> <port> > <output_file>
dec_server:

bash
Copy code
$ ./dec_server <listening_port> &
dec_client:

bash
Copy code
$ ./dec_client <ciphertext_file> <key_file> <port> > <output_file>
keygen:

bash
Copy code
$ ./keygen <key_length> > <key_file>
Testing
Use the provided test script p5testscript to evaluate the correctness of your implementation. Ensure to change port numbers for each test run to avoid conflicts.

bash
Copy code
$ ./p5testscript
Notes
Ensure that plaintext and key files contain only valid characters (A-Z and space).
enc_client should not be able to connect to dec_server, and vice versa.
Handle errors gracefully and output appropriate messages to stderr.

README
Files can only be compiled only with -std=c99
This is provided in my compileall file



TO RUN THE PROGRAM:

1. Run compileall

2. Or run these commands:
gcc -std=c99 enc_server.c -o enc_server
gcc -std=c99 enc_client.c -o enc_client
gcc -std=c99 dec_server.c -o dec_server
gcc -std=c99 dec_client.c -o dec_client
gcc -std=c99 keygen.c -o keygen
