#include <cstdlib>
#include <iostream>

#include "../constants.h"
#include "../enet.h"

int main() {
    if (enet_initialize()) {
        std::cerr << "An error occurred while initializing ENet.\n";
        std::exit(EXIT_FAILURE);
    }
    std::atexit(&enet_deinitialize);

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = server::port;

    // static storage lets the atexit lambda access server w/o taking it as an argument
    static ENetHost* server = enet_host_create(&address, server::max_peers, server::channels, server::incoming_bandwidth, server::outgoing_bandwidth);
    if (!server) {
        std::cerr << "An error occurred while trying to create an ENet server host.\n";
        std::exit(EXIT_FAILURE);
    }
    std::atexit([] { enet_host_destroy(server); });

    // PROGRAM LOOP START

    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, server::event_idle_timeout_ms) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A client connected from %x:%hu.\n",
                           event.peer->address.host,
                           event.peer->address.port);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%x:%hu disconnected.\n",
                           event.peer->address.host,
                           event.peer->address.port);
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    printf("A packet of length %zu containing %s was received from %x:%hu on channel %u.\n",
                           event.packet->dataLength,
                           event.packet->data,
                           event.peer->address.host,
                           event.peer->address.port,
                           event.channelID);
                    break;
            }
        }
    }

    // PROGRAM LOOP END

    // TODO: add a way to exit the loop

    exit(EXIT_SUCCESS);
}