#include <set>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL_net.h>

#include <server/packages.h>
#include <server/client.h>

Client* establishConnection(SDLNet_SocketSet& set, TCPsocket server)
{
    TCPsocket client = SDLNet_TCP_Accept(server);
    if (client == NULL)
        return NULL;

    Client *newClient = new Client(client);

    newClient->sendPackage(ID);
    newClient->sendPackage(newClient->id);

    connSignal sig;
    newClient->recievePackage(&sig);
    if(sig == OK)
    {
        SDLNet_TCP_AddSocket(set, client);
        printf("[server] %s logged in!\n", newClient->name);
    }
    else
    {
        delete newClient;
        newClient = NULL;
        printf("[server] connection failed!\n");
    }

    return newClient;
}

int main(int argc, char* argv[])
{
    printf("Starting TicTacToe server...\n");

    SDLNet_Init();

    IPaddress address;
    TCPsocket server;

    if (SDLNet_ResolveHost (&address, NULL, 12345) == -1)
    {
        printf ("[server] SDLNet_ResolveHost: %s\n", SDLNet_GetError ());
        SDLNet_Quit ();
        exit (1);
    }

    server = SDLNet_TCP_Open (&address);
    if (server == NULL)
    {
        printf ("[server] TCP_Open: %s\n", SDLNet_GetError ());
        SDLNet_Quit ();
        exit (-1);
    }

    std::set<Client*> clients;
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(10);

    while(1)
    {
        Client* c = establishConnection(set, server);
        if(c != NULL)
        {
            clients.insert(c);
        }

        if(SDLNet_CheckSockets(set, 10))
        {
            for(Client* c : clients)
                c->handle(clients, set);
        }

        usleep(1000);
    }

    return 0;
}
