#include <stdio.h>
#include <string.h>
#include "enet/enet.h"
#include "includeAll.h"


#define MAX_CLIENTS 2

ENetPeer *clients[MAX_CLIENTS] = {0};
int clientCount = 0;

int main ()
{
	if (enet_initialize () != 0)
	{
		fprintf (stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetEvent event;
	ENetAddress address;
	ENetHost *server;

	enet_address_set_host(&address, "127.0.0.1");
	address.port = 1234;



	server = enet_host_create (&address	/* the address to bind the server host to */,
					2	/* allow up to 2 clients and/or outgoing connections */,
					1	/* allow up to 2 channels to be used, 0 & 1 */,
					0	/* assume any amount of incoming bandwidth */,
					0	/* assume any amount of outgoing bandwidth */);

	if (server == NULL)
	{
		printf("An error occurred while trying to create an ENet server host.");
		return 1;
	}

	// gameloop
	while(1)
	{
		ENetEvent event;
		/* Wait up to 1 sec for an event. */
		while (enet_host_service(server, &event, 1000) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
					printf ("A new client connected from %x:%u. ConnectID: %u\n",
					event.peer -> address.host,
					event.peer -> address.port,
					event.peer->connectID);
					if(clientCount < MAX_CLIENTS)
					{
						clients[clientCount] = event.peer;
						clientCount++;
					}
				break;

				case ENET_EVENT_TYPE_RECEIVE:
					//printf ("%u: %s\n", event.peer->connectID, event.packet->data);
						// broadcast that data to the rest of the clients
						for(int i = 0; i < clientCount; i++)
						{
							if(event.peer->connectID != clients[i]->connectID)
							{
								ENetPacket *packet = enet_packet_create(event.packet->data, strlen(event.packet->data) + 1, ENET_PACKET_FLAG_RELIABLE);
    							if(packet != NULL)
								{
									enet_peer_send(clients[i], 0, packet);
									//printf("Sending packet with data %s to %u\n", event.packet->data, clients[i]->connectID);
								}
							}
						}
						/* Clean up the packet now that we're done using it. */
						enet_packet_destroy (event.packet);
				break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf ("%s disconnected.\n", event.peer -> data);
					/* Reset the peer's client information. */
					event.peer -> data = NULL;
					
                break;
			}
		}
	}

	enet_host_destroy(server);

	return 0;
}