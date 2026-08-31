#include "network.h"
#include <stdio.h>
#include "enet/enet.h"
#include "includeAll.h"
#include <string.h>

#define N_SUCCESS 1
#define N_FAILED 0

/*** change the max clients number here to change how many clients the server allows ***/
#define MAX_CLIENTS 32
#define CHANNEL_COUNT 2

void addClient(ENetPeer *newClient);
void removeClient(ENetPeer *removeClient);
void destroyServerOrClient(ENetHost *host);

// for keeping track of connected peers for a server
ENetPeer *clients[MAX_CLIENTS] = {0};
int num_clients = 0;

ENetHost *server_or_client = {0}; // the server or client, depending on what's called here
ENetAddress address = {0};
ENetEvent event = {0};
ENetPeer *peer = {0};
ENetPacket *packet = {0};

unsigned int num_events[10] = {0};
unsigned int num_averageEvents = 0;
int iterator = 0;

// keeping track if we are a server or a client
/*
    0 - not set yet
    -1 - server
    -2 - client
*/
typedef enum hostTracker
{
    n_notSet = 0,
    n_server = -1,
    n_client = -2
}hostTracker;
hostTracker b_server_or_client = n_notSet;
int b_createdHost = 0;


// ### client ###
//  connect to server
//  can't become a server while a client
//  send data to the server
//  receive data from the server


// ### server ###
//  become a server accepting connections
//  can't connect to other servers while a server
//  send data to clients
//  receive data from clients
//  manage logic and important bits

// ### together ###
//  can't become the other while a client or server
//  send data
//  receive data

// initialize the network
// returns 1 if succeeded, returns 0 if failed
int network_initialize()
{
    if(enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet!\n");
		return N_FAILED; // return false
	}
    b_createdHost = 1; // successfully created host
    return N_SUCCESS; // return true
}

// become a server
// returns 1 if succeeded, 0 if not, otherwise it will return whether it is a client or a server
int network_createServer(const char *IPAddress, const int port)
{
    // check if we are already a server or client
    if(b_server_or_client != n_notSet)
    {
        return b_server_or_client;
    }
    enet_address_set_host(&address, IPAddress);
    address.port = port;
    // setup the the server information
    server_or_client = enet_host_create(&address, MAX_CLIENTS, CHANNEL_COUNT, 0, 0);
    if(server_or_client == NULL)
    {
    	fprintf(stderr, "An error occurred while trying to create an ENet server!\n");
        return N_FAILED;
    }
    b_server_or_client = n_server;

    return N_SUCCESS;
}

// connect to a server
// returns 1 if succeeded, 0 if not, otherwise it will return whether it is a client or a server
int network_connect(const char *IPAddress, const int port)
{
    // check if we are already a server or client
    if(b_server_or_client != n_notSet)
    {
        return b_server_or_client;
    }
    // setup the the client information
    server_or_client = enet_host_create(NULL, 1, CHANNEL_COUNT, 0, 0);
    if(server_or_client == NULL)
    {
    	fprintf(stderr, "An error occurred while trying to create an ENet client host!\n");
    	return N_FAILED;
    }
    // set the address and port of the server we are going to connect to
    enet_address_set_host(&address, IPAddress);
    address.port = port;

    // attempted to connect to the server
    peer = enet_host_connect(server_or_client, &address, CHANNEL_COUNT, 0);
    if(peer == NULL)
    {
        // failed connecting due to no servers available with this IP address and port
    	fprintf(stderr, "No available peers for initiating an ENet connection!\n");
        enet_host_destroy(server_or_client);
    	return N_FAILED;
    }
    // connected to the server, awaiting a response from the server acknowleding our connection
    if(enet_host_service(server_or_client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        // we connected!
    	puts("Connection to server succeeded.");
    }
    else
    {
        // server didn't respond, resetting peer
        enet_host_destroy(server_or_client);
    	enet_peer_reset(peer);
    	puts("Connection to server failed.");
    	return N_FAILED;
    }

    b_server_or_client = n_client;

	return N_SUCCESS;
}

// send data
void network_sendData(const char *data)
{
    packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    if(b_server_or_client == -1) // we are a server sending info to our clients
    {
        enet_host_broadcast(server_or_client, 1, packet);
    }
    else if(b_server_or_client == -2) // sending info from client to server
    {
        enet_peer_send(peer, 0, packet);
    }
}

// receive data
// needs to be ran every frame
// returns the following
//  * 1 if a client connected to the server
//  * 2 if a client disconnected from the server
//  * 3 if we received a packet -> this packet data will be stored in "storeData"
int network_getEvents(char *storeData)
{
    int message = ENET_EVENT_TYPE_NONE;
    while(enet_host_service(server_or_client, &event, 0) > 0)
	{
        if(event.type == ENET_EVENT_TYPE_CONNECT) // received a connection from a client
        {
            printf ("A new client connected from %x:%u. ConnectID: %u\n", event.peer->address.host, event.peer->address.port, event.peer->connectID);
			addClient(event.peer);
            enet_packet_destroy(event.packet);
            message = ENET_EVENT_TYPE_CONNECT;
            //return ENET_EVENT_TYPE_CONNECT;
        }

        if(event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            // fill the dataStorage with the new data
            strcpy(storeData, event.packet->data);
            //storeSendingPeer = event.peer; // store the peer so we know who sent the data
            enet_packet_destroy(event.packet);
            message = ENET_EVENT_TYPE_RECEIVE;
            //return ENET_EVENT_TYPE_RECEIVE;
        }

        if(event.type == ENET_EVENT_TYPE_DISCONNECT)
        {
            printf ("%u disconnected.\n", event.peer -> connectID);
            removeClient(event.peer);
			/* Reset the peer's client information. */
			event.peer -> data = NULL;
            enet_packet_destroy(event.packet);
            message = ENET_EVENT_TYPE_DISCONNECT;
            //return ENET_EVENT_TYPE_DISCONNECT;
        }		
	}
    return message;
}

// if client - disconnects client from server
// if server - closes the server
int network_disconnect()
{
    // if we are not a server or a client then there is nothing to disconnect from
    if(b_server_or_client == n_notSet)
    {
        return b_server_or_client;
    }
    if(b_server_or_client == n_client)
    {
        // request a graceful disconnect request
        enet_peer_disconnect(peer, 0);
        // wait for confirmation from the server stating we are disconnected
        while(enet_host_service(server_or_client, &event, 3000) > 0)
        {
        	switch(event.type)
        	{
        		case ENET_EVENT_TYPE_RECEIVE:
                    // clearing events in the buffer we don't need
        			enet_packet_destroy(event.packet);
        			break;
        		case ENET_EVENT_TYPE_DISCONNECT:
        			puts("Disconnection succeeded.");
        			break;
        	}
        }
    }
    else if(b_server_or_client == n_server)
    {
        for(int i = 0; i < num_clients; i++)
        {
            enet_peer_disconnect(clients[i], 0);
        }
    }

    // destroy the client server configuration after disconnecting
    destroyServerOrClient(server_or_client);
    return N_SUCCESS;
}

// close the network
void network_deinitialize()
{
    if(b_createdHost == 1) // if the network was initialized
    {
        enet_deinitialize();
        b_createdHost = 0;
    }
}

// checks if we are the server or the client in this relationship
/*
    0 - not set yet
    -1 - server
    -2 - client
*/
int network_serverOrClient()
{
    return b_server_or_client;
}

void destroyServerOrClient(ENetHost *host)
{
    enet_host_destroy(host);
    b_server_or_client = n_notSet;
}

// add client to the clients list
void addClient(ENetPeer *newClient)
{
    if(num_clients < MAX_CLIENTS)
    {
        // add the  client
        clients[num_clients] = newClient;
        num_clients++;
    }
}

// remove clients from the clients list
void removeClient(ENetPeer *removeClient)
{
    int i = 0; // index of the client to remove
    int found = 0;
    for(; i < num_clients; i++)
    {
        if(clients[i]->connectID == removeClient->connectID)
        {
            found = 1; // we found the client to remove
            break;
        }
    }
    if(found == 1) // make sure we find the client to remove
    {
        for(; i < (num_clients - 1); i++)
        {
            // shift the array down, removing the client
            clients[i] = clients[i + 1];
        }
        num_clients--;
    }
}

unsigned int network_getEventListLength()
{
    if(b_server_or_client == n_notSet)
    {
        return 0;
    }

    iterator = (iterator + 1)%10;
    num_averageEvents -= num_events[iterator];
    num_events[iterator] = enet_list_size(&(server_or_client->dispatchQueue));
    num_averageEvents += num_events[iterator];

    return num_averageEvents/10;
}