How to compile and run:
run the following statements in terminal to compile both files:

gcc -o server server.c
gcc -o client client.c

now that both files are compiled, we need run both files (the server always goes first)
run the server by typing ./server " fallowed by any number between 2000 and 65535. that 
number represents the port number. An example would be:

./server 51718

Now it is time to run the client. The client is run by ./client " fallowed by the host name
of the computer running the server. This can be found by entering hostname on the serverside terminal.
after the hostname you need the same port number the server used to run. An example is below:

./client localhost 51718

now both sides should be up and running and ready to chat. The client side will exit if you enter
!EXIT but the server side will continue to run until it is closed though control C. The Client always 
sends the first message and the server may respond. 