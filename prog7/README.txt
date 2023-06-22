Creighton Cathey
CS3304
Prog 9 

TLDR: 
make
server 1234
*new terminal window
client 127.0.0.1 1234 
*new terminal window
client 127.0.0.1 1234

This program is a client-server chat application that allows multiple clients to connect to a server and communicate with each other in real-time.

How it works

The server listens for incoming connections from clients on a specified port. When a client connects to the server, the server sends a welcome message to the client and prompts the client to enter a username. Once the username is entered, the client can send messages to the server, which are then broadcasted to all other clients connected to the server.

How to use

First make the files with the newest versions using the makefile by running 
"make" 

Then you can start the server by running 
"server <port>" 
Example: 
server 1234
<port> is the port number on which the server should listen for incoming connections.

Once the server is running, clients can connect to it by running the following command 
"client <IP> <port>"
Example:
client 127.0.0.1 1234
If you are running the programs on the same machine you can use the ip in the example 
If not, use the ip of the server 
After connecting to the server, the client will be prompted to enter a username. 
You can use any username. 

To quit the app, type \q into the command prompt and press enter.

Features

The following features were implemented in the program:

Multiple clients can connect to a server and communicate with each other in real-time
The server broadcasts messages sent by clients to all other connected clients
Clients can enter a username when connecting to the server
The server sends a welcome message to new clients when they connect
Clients can quit the app by typing \q into the command prompt
The clients get of input from the user, send it to the server and receive replies from the server in a loop. 
Clients do not interact directly. 
Threads are implemented in the client and the server. 
Basic error checking. 
Server and client ports are set to a fixed value 
The server accepts connections in a loop and spawns a thread for each connected client, it can handle more than 2 clients. 
The server stores the username and associates it with the client socket. 
The character "\" is only for commands. 

Limitations or bugs: 
I noticed sometimes the client could not connect to the server when using “localhost” as the ip, but using 127.0.0.1 has worked 
