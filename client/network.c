#include "network.h"
#include <stdio.h>
#include "enet/enet.h"
#include "includeAll.h"
#include <string.h>


ENetHost *client = {0};
ENetAddress address = {0};
ENetEvent event = {0};
ENetPeer *peer = {0};
ENetPacket *packet = {0};

int network_connect()
{
	if(enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet!\n");
		return EXIT_FAILURE;
	}
    client = enet_host_create(NULL, 1, 1, 0, 0);

    if(client == NULL)
    {
    	fprintf(stderr, "An error occurred while trying to create an ENet client host!\n");
    	return EXIT_FAILURE;
    }
    
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;

    peer = enet_host_connect(client, &address, 1, 0);
    if(peer == NULL)
    {
    	fprintf(stderr, "No available peers for initiating an ENet connection!\n");
    	return EXIT_FAILURE;
    }
    if(enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
    	puts("Connection to 127.0.0.1:1234 succeeded.");

    }
    else
    {
    	enet_peer_reset(peer);
    	puts("Connection to 127.0.0.1:1234 failed.");
    	return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}

int network_process(char *data)
{
    int receivedData = 0;
    while(enet_host_service(client, &event, 0) > 0)
    {
        receivedData = 1;
        if(event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            strcpy(data, event.packet->data);
        }

        /* Clean up the packet now that we're done using it. */
        enet_packet_destroy (event.packet);
    }

    return receivedData;
}

void network_sendPacket(const char *packetInfo)
{
    packet = enet_packet_create(packetInfo, strlen(packetInfo) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void network_disconnect()
{
    enet_peer_disconnect(peer, 0);

    while(enet_host_service(client, &event, 3000) > 0)
    {
    	switch(event.type)
    	{
    		case ENET_EVENT_TYPE_RECEIVE:
    			enet_packet_destroy(event.packet);
    			break;
    		case ENET_EVENT_TYPE_DISCONNECT:
    			puts("Disconnection succeeded.");
    			break;
    	}
    }
}

void network_deinitialize()
{
    enet_host_destroy(client);
    enet_deinitialize();
}