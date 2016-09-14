#include <set>
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

        void logout(std::set<Client*>& playerlist, SDLNet_SocketSet& set)
        {
            playerlist.erase(this);
            SDLNet_TCP_AddSocket(set, this->connection);
            SDLNet_TCP_Close(this->connection);
        }

        char name[64];
        TCPsocket connection;

        unsigned int id;
        static unsigned int id_counter;

        LoginPackage genPackagage()
        {
            LoginPackage p;
            strcpy(p.name, this->name);
            p.id = this->id;
            return p;
        }

        void handle(std::set<Client*>& playerlist, SDLNet_SocketSet& set)
        {
            connSignal sig;
            if(SDLNet_SocketReady(this->connection))
            {
                recievePackage(this->connection, &sig);
                switch(sig)
                {
                    case LOGOUT:
                        this->logout(playerlist, set);
                        printf("%s logged off\n", this->name);
                        delete this;
                        return;

                    case REQ_PLAYERLIST:
                        uint32_t num_players = playerlist.size();
                        sendPackage(this->connection, &num_players);

                        for(Client* c : playerlist)
                        {
                            LoginPackage p = c->genPackagage();
                            sendPackage(this->connection, &p);
                        }
                        break;
                }

                recievePackage(this->connection, &sig);
                if(sig != OK)
                {
                    printf("client %s doesn't respond.. kick him!\n", this->name);
                    this->logout(playerlist, set);
                    delete this;
                    return;
                }
            }
        }
};

unsigned int Client::id_counter = 0;

Client* establishConnection(SDLNet_SocketSet& set, TCPsocket server)
{
    TCPsocket client = SDLNet_TCP_Accept(server);
    if (client == NULL)
        return NULL;

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
        SDLNet_TCP_AddSocket(set, client);
        printf("established connection with %s!\n", newClient->name);
    }
    else
    {
        delete newClient;
        newClient = NULL;
        printf("connection failed!\n");
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
