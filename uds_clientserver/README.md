UNIX Domain Socket Client-Server Example in C
Overview

This project demonstrates Inter-Process Communication (IPC) using UNIX Domain Sockets (AF_UNIX) in Linux.

The application consists of:

Server (server.c)
Client (client.c)

The client sends a sequence of integer values to the server. The server accumulates (sums) all received values and returns the final result back to the client.

Communication is performed using a UNIX domain stream socket located at:

/tmp/mySocket
Features
UNIX Domain Socket communication (AF_UNIX)
Connection-oriented communication (SOCK_STREAM)
Blocking I/O using read() and write()
Single-threaded server implementation
Demonstrates:
 socket()
 bind()
 listen()
 accept()
 connect()
 read()
 write()
 close()
 unlink()
Architecture
+---------+                        +---------+
| Client  |                        | Server  |
+---------+                        +---------+
     |                                  |
     |------ connect() ---------------->|
     |                                  |
     |------ Integer Data ------------->|
     |------ Integer Data ------------->|
     |------ Integer Data ------------->|
     |------ 0 (End Marker) ----------->|
     |                                  |
     |<------ Result String ------------|
     |                                  |
How It Works
 Server
  Removes any previously existing socket file.
  Creates a UNIX stream socket.
  Binds the socket to /tmp/mySocket.
  Starts listening for incoming client connections.
  Accepts a client connection.
  Receives integers from the client.
  Adds received values to a running total.
  Stops receiving when value 0 is received.
  Sends the final sum back to the client.
  Waits for the next client connection.
 Client
  Creates a UNIX stream socket.
  Connects to /tmp/mySocket.
  Prompts the user to enter integers.
  Sends each integer to the server.
  Terminates input by entering 0.
  Requests the result from the server.
  Receives and displays the final sum.
  Closes the connection.
  Build Instructions

Compile the server and client using GCC:

gcc server.c -o server
gcc client.c -o client

Running the Application
Terminal 1 - Start Server
./server

Expected output:

Master socket created
bind() call succeed
Waiting on accept() sys call
Terminal 2 - Start Client
./client

Example:

Enter number to send to server :
10

Enter number to send to server :
20

Enter number to send to server :
30

Enter number to send to server :
0

Client output:

Result = Result = 60

Server output:

Connection accepted from client
Waiting for data from the client
Waiting for data from the client
Waiting for data from the client
sending final result back to client
Socket API Flow
Server Side
socket()
   |
bind()
   |
listen()
   |
accept()
   |
read()
   |
write()
   |
close()
Client Side
socket()
   |
connect()
   |
write()
   |
read()
   |
close()

Key Concepts Demonstrated
UNIX Domain Sockets

UNIX Domain Sockets provide IPC between processes running on the same machine.

Advantages:

Faster than TCP/IP sockets
No network stack overhead
Secure local communication
Suitable for daemon-process communication

Examples:

DLT Daemon
Docker Engine
systemd services
Database local connections
Limitations

Current implementation is intended for learning purposes.

Limitations include:

Single client processed at a time
No multithreading
No timeout handling
No graceful shutdown mechanism
Minimal error recovery
Assumes same machine communication
Possible Enhancements
Multi-client support using threads
Support for poll() or select()
Non-blocking sockets
Structured message protocol
Graceful server shutdown
Client authentication
Logging support

Learning Objectives

This project helps understand:

Linux IPC mechanisms
UNIX Domain Sockets
Blocking socket communication
Client-server architecture
Linux socket programming fundamentals
Author

Created as a learning example for Linux Socket Programming and Inter-Process Communication (IPC).
