README
Files can only be compiled only with -std=c99
This is provided in my compileall file
Everything should work except for the extreamly large file and when enc and dec try to use the same port.
Grading script works on my local machine except for the afore mentioned problems. Thank you have a good summer!



TO RUN THE PROGRAM:

1. Run compileall

2. Or run these commands:
gcc -std=c99 enc_server.c -o enc_server
gcc -std=c99 enc_client.c -o enc_client
gcc -std=c99 dec_server.c -o dec_server
gcc -std=c99 dec_client.c -o dec_client
gcc -std=c99 keygen.c -o keygen

NOTE: I could not get the compileall to work when I tried running the grading script. I have also included the precompiled files in the zip incase this is a problem for you too.