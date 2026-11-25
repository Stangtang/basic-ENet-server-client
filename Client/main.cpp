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

    // pass NULL address to create client host and not server host
    // static storage lets the atexit lambda access client w/o taking it as an argument
    static ENetHost* client = enet_host_create(NULL, client::outgoing_connections, client::channels, client::incoming_bandwidth, client::outgoing_bandwidth);
    if (!client) {
        std::cerr << "An error occurred while trying to create an ENet client host.\n";
        std::exit(EXIT_FAILURE);
    }
    std::atexit([] { enet_host_destroy(client); });

    ENetAddress address;
    address.port = server::port;
    if (enet_address_set_host(&address, server::host_name)) {
        std::cerr << "Failed to resolve server hostname '" << server::host_name << "'.\n";
        std::exit(EXIT_FAILURE);
    }

    // the server is the peer
    // pass 0 to not send data data upon connection
    ENetPeer* peer = enet_host_connect(client, &address, client::channels, 0);
    if (!peer) {
        std::cerr << "No available peers for initiating an ENet connection.\n";
        std::exit(EXIT_FAILURE);
    }

    // check if server has responded
    ENetEvent event;
    if (enet_host_service(client, &event, client::connection_wait_ms) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to " << server::host_name << ':' << server::port << " succeeded.\n";
    } else {
        enet_peer_reset(peer);
        std::cout << "Connection to " << server::host_name << ':' << server::port << " failed.\n";
        std::exit(EXIT_SUCCESS);
    }

    // PROGRAM LOOP START

    while (enet_host_service(client, &event, client::loop_wait_ms) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                // use printf as it is much easier
                printf("A packet of length %zu containing %s was received from %x:%hu on channel %u.\n",
                       event.packet->dataLength,
                       event.packet->data,
                       event.peer->address.host,
                       event.peer->address.port,
                       event.channelID);
                break;
        }
    }

    // PROGRAM LOOP END

    // pass 0 to send no data when disconnecting
    enet_peer_disconnect(peer, 0);
    while (enet_host_service(client, &event, client::disconnect_wait_ms) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                // discard any packets received while waiting for disconnection to finish
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnection succeeded.\n";
                break;
        }
    }

    exit(EXIT_SUCCESS);
}