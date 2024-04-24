# One-Time Pad Encryption System

This repository contains a set of C programs designed to implement a one-time pad-like encryption and decryption system using socket-based inter-process communication (IPC).

## Introduction

In this assignment, you will be creating five small programs that encrypt and decrypt information using a one-time pad-like system. These programs will combine the multi-processing code you have been learning with socket-based inter-process communication. Your programs will also be accessible from the command line using standard Unix features like input/output redirection, and job control. Finally, you will write a short compilation script.

## Learning Outcomes

After successful completion of this assignment, you should be able to:
- Compare and contrast IPC facilities for communication
- Explain the Client-Server communication model at a high level
- Understand and use the programmer’s view of the internet to design network programs
- Explain the concept of Unix sockets
- Design and implement client and server programs for IPC using sockets
- Compare and evaluate designs for servers

## One-Time Pads

Refer to the [Wikipedia page on One-Time Pads](https://en.wikipedia.org/wiki/One-time_pad) for details on how one-time pads work.

## Specifications

### Programs
- **enc_server:** Encryption server that listens on a specified port, receives plaintext and key from `enc_client`, performs encryption, and sends back ciphertext.
- **enc_client:** Connects to `enc_server`, sends plaintext and key files, and receives ciphertext.
- **dec_server:** Decryption server that listens on a specified port, receives ciphertext and key from `dec_client`, performs decryption, and sends back plaintext.
- **dec_client:** Connects to `dec_server`, sends ciphertext and key files, and receives plaintext.
- **keygen:** Generates a key file of specified length containing random characters (A-Z and space).

### Usage

#### Compilation

Compile all programs using the provided `compileall` script or Makefile:

```bash
#!/bin/bash
gcc -o enc_server enc_server.c
gcc -o enc_client enc_client.c
gcc -o dec_server dec_server.c
gcc -o dec_client dec_client.c
gcc -o keygen keygen.c

### Running Programs

Start `enc_server` and `dec_server` in the background on different ports:

```bash
$ ./enc_server 57171 &
$ ./dec_server 57172 &
Use enc_client and dec_client to encrypt and decrypt text:

bash
Copy code
$ enc_client plaintext.txt key.txt 57171 > ciphertext.txt
$ dec_client ciphertext.txt key.txt 57172 > decrypted.txt
Ensure to replace plaintext.txt and key.txt with your actual input filenames, and the port numbers (57171, 57172) should match the ports where your enc_server and dec_server are listening respectively.

### Notes

- Ensure that each program handles errors gracefully and outputs error messages to `stderr`.
- Use `localhost` as the target IP address/host for all networking connections.
