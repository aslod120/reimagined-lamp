#ifndef NETWORK
#define NETWORK

int network_connect();
int network_process(char *data);
void network_sendPacket(const char *packetInfo);
void network_disconnect();
void network_deinitialize();

#endif