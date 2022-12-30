#pragma once

#include <chrono>

// Default UDP timeout
#define UDP_TIMEOUT std::chrono::milliseconds(1000)

// Allow use of network simulator
#define NETWORK_SIMULATOR 1

#ifdef _DEBUG

// Allow debugging network applications : when a client times out, consider its connection interrupted rather than disconnected so he can come back and resume it
#define NETWORK_INTERRUPTION 1

#endif // _DEBUG

#ifndef UDP_TIMEOUT
#define UDP_TIMEOUT std::chrono::seconds(1)
#endif

#ifndef NETWORK_SIMULATOR
#define NETWORK_SIMULATOR 0
#endif

#ifndef NETWORK_INTERRUPTION
#define NETWORK_INTERRUPTION 0
#endif