#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL_net.h>

#include <server/packages.h>

class Client
{
    public:
        Client(char const* const name_, TCPsocket connection_)
            : connection(connection_)
        {
            strcpy(this->name, name_);
            id = id_counter++;
        }
        ~Client();

        char name[64];
        TCPsocket connection;

        unsigned int id;
        static unsigned int id_counter;
};

unsigned int Client::id_counter = 0;

void establishConnection(SDLNet_SocketSet *set, TCPsocket server, std::vector<Client*>& clients)
{
    TCPsocket client = SDLNet_TCP_Accept(server);
    if (client == NULL)
        return;

    printf("got a connection\n");

    LoginPackage login;
    recievePackage(client, &login);

    Client *newClient = new Client(login.name, client);

    connSignal sig = ID;
    sendPackage(client, &sig);
    sendPackage(client, &newClient->id);

    recievePackage(client, &sig);
    if(sig == OK)
    {
        clients.push_back(newClient);
        SDLNet_TCP_AddSocket(*set, client);

        printf("connection with %s established!\n", newClient->name);
    }
    else
        printf("connection failed!\n");

}

int main(int argc, char* argv[])
{
    printf("Starting TicTacToe server...\n");

    SDLNet_Init();

    IPaddress address;
    TCPsocket server;

    if (SDLNet_ResolveHost (&address, NULL, 12345) == -1)
    {
        printf ("SDLNet_ResolveHost: %s\n", SDLNet_GetError ());
        SDLNet_Quit ();
        exit (1);
    }

    server = SDLNet_TCP_Open (&address);
    if (server == NULL)
    {
        printf ("ERR TCP_Open: %s\n", SDLNet_GetError ());
        SDLNet_Quit ();
        exit (-1);
    }

    std::vector<Client*> clients;
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(10);

    while(1)
    {
        establishConnection(&set, server, clients);
        usleep(100);
    }

    return 0;
}
