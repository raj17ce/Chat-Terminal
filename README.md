# Chat Terminal

A terminal-based LAN chat application written in C++ using raw POSIX sockets. Run a server, connect multiple clients, and chat in real time.

## How it works

The server listens on a port you choose and accepts multiple clients concurrently using a thread pool (8 threads). Each client picks a username on connect. Every message is broadcast to all other connected clients.

## Build

Requires CMake 3.16+ and a C++17 compiler.

```bash
cmake -S . -B build
cmake --build build
```

## Run

Start the server first:
```bash
./build/Server/Server
# Enter port: 17000
```

Then connect clients (on the same machine or LAN):
```bash
./build/Client/Client
# Enter IP: 127.0.0.1
# Enter port: 17000
# Enter chat name: Alice
```

Type `/quit` to disconnect a client.
