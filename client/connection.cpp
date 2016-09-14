
#include <cstdint>
#include <SDL2/SDL_net.h>
#include <server/packages.h>

#include <connection.h>

Connection::Connection(char const* const servername, uint16_t serverPort, char const* const username)
{
    SDLNet_ResolveHost(&this->server_address, servername, serverPort);
    this->server = SDLNet_TCP_Open(&this->server_address);
    if(!this->server)
    {
        printf("could not connect to server\n");
        exit(-1);
    }

    LoginPackage login;
    strcpy(login.name, username);
    sendPackage(this->server, &login);

    connSignal sig;
    recievePackage(this->server, &sig);
    if(sig == ID)
    {
        recievePackage(this->server, &this->id);
        connSignal sig = OK;
        sendPackage(this->server, &sig);

        this->set = SDLNet_AllocSocketSet(1);
        if(SDLNet_TCP_AddSocket(this->set, this->server) == -1)
        {
            printf("Unable to add server-socket to sockset\n");
            exit(-1);
        }
    }
    else
        printf("error: couldn't recieve ID\n");
}

Connection::~Connection()
{
    connSignal sig = LOGOUT;
    sendPackage(this->server, &sig);

    SDLNet_TCP_Close(this->server);
}

void Connection::getPlayers(void)
{
    printf("requesting player list..\n");

    connSignal sig = REQ_PLAYERLIST;
    sendPackage(this->server, &sig);

    uint32_t nplayers;
    recievePackage(this->server, &nplayers);

    for(int i=0; i < nplayers; i++)
    {
        LoginPackage p;
        recievePackage(this->server, &p);
        this->playerlist.push_back(p);

        printf("* %s\n", p.name);
    }

    sig = OK;
    sendPackage(this->server, &sig);
}
