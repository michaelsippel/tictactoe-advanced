#include <map>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL_net.h>

#include <server/packages.h>
#include <server/client.h>

Client* establishConnection(SDLNet_SocketSet& set, TCPsocket client, uint32_t id)
{
    Client *newClient = new Client(client);

    newClient->sendPackage(ID);
    newClient->sendPackage(id);

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

    if (SDLNet_ResolveHost (&address, NULL, 2365) == -1)
    {
        printf ("[server] SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        exit (1);
    }

    server = SDLNet_TCP_Open (&address);
    if (! server)
    {
        printf ("[server] TCP_Open: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        exit (-1);
    }

    std::map<uint32_t, Client*> clients;
    std::map<uint32_t, Game*> games;
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(10);

    uint32_t id_counter = 0;
    while(1)
    {
        usleep(2000000);

        // add new clients
        TCPsocket socket = SDLNet_TCP_Accept(server);
        if(socket != NULL)
        {
            uint32_t id = id_counter++;
            Client* c = establishConnection(set, socket, id);
            clients.insert(std::pair<uint32_t, Client*>(id, c));
        }

        // handle clients
        if(SDLNet_CheckSockets(set, 10))
        {
            for(auto it = clients.begin(); it != clients.end(); ++it)
                it->second->handle(it, clients, games, set);
        }
    }

    SDLNet_Quit();
    return 0;
}
