#ifndef NETWORK
#define NETWORK

// initialize the network
// returns 1 if succeeded, returns 0 if failed
int network_initialize();

// become a server
// returns 1 if succeeded, 0 if not, otherwise it will return whether it is a client or a server
int network_createServer(const char *IPAddress, const int port);

// connect to a server
// returns 1 if succeeded, 0 if not, otherwise it will return whether it is a client or a server
int network_connect(const char *IPAddress, const int port);

// send data
// if we are a client then we are sending data to the server
// if we are a server then we are broadcasting data out to all clients
void network_sendData(const char *data);

// receive data
// needs to be ran every frame
// returns the following
//  * 1 if a client connected to the server
//  * 2 if a client disconnected from the server
//  * 3 if we received a packet -> this packet data will be stored in "storeData"
int network_getEvents(char *storeData);

// if client - disconnects client from server
// if server - closes the server
int network_disconnect();

// close the network
void network_deinitialize();

// checks if we are the server or the client in this relationship
/*
    0 - not set yet
    -1 - server
    -2 - client
*/
int network_serverOrClient();

#endif