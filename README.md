# Basic ENet Server and Client

A minimal C++ example of a client connecting to a server using ENet. I made this as a jumping-off point for any future projects I might build with ENet.

The server code is in [`Server/main.cpp`](Server/main.cpp), and the client code is in [`Client/main.cpp`](Client/main.cpp).

## Build and run

ENet is included in the repo.

On windows, you may need to add linker flags `-lws2_32 -lwinmm` to the build commands.