#pragma once

#include <cstddef>

#include "enet.h"

namespace server {
inline constexpr const char* host_name = "127.0.0.1";
inline constexpr enet_uint16 port = 9999;
inline constexpr size_t max_peers = 32;
inline constexpr size_t channels = 1;
inline constexpr enet_uint32 incoming_bandwidth = 0, outgoing_bandwidth = 0;  // do not limit bandwidth
inline constexpr enet_uint32 event_idle_timeout_ms = 1000;
}  // namespace server

namespace client {
inline constexpr size_t outgoing_connections = 1;
inline constexpr size_t channels = 1;
inline constexpr enet_uint32 incoming_bandwidth = 0, outgoing_bandwidth = 0;  // do not limit bandwidth
inline constexpr enet_uint32 connect_timeout_ms = 5000, disconnect_timeout_ms = 3000, event_idle_timeout_ms = 1000;
}  // namespace client