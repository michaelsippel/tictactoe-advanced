
#include <cstdint>
#include <SDL2/SDL_net.h>
#include <server/packages.h>

#include <connection.h>

Connection::Connection(char const* const servername, uint16_t serverPort, char const* const username)
    : Socket(setupSocket(servername, serverPort))
{
    LoginPackage login;
    strcpy(login.name, username);
    sendPackage(login);

    connSignal sig;
    recievePackage(&sig);
    if(sig == ID)
    {
        recievePackage(&this->id);
        sendPackage(OK);

        this->set = SDLNet_AllocSocketSet(1);
        if(SDLNet_TCP_AddSocket(this->set, this->sock) == -1)
        {
            printf("Unable to add server-socket to sockset\n");
            exit(-1);
        }
    }
    else
        printf("error: couldn't recieve ID\n");
}

TCPsocket Connection::setupSocket(char const* const servername, uint16_t serverPort)
{
    IPaddress server_address;
    SDLNet_ResolveHost(&server_address, servername, serverPort);
    TCPsocket socket = SDLNet_TCP_Open(&server_address);
    if(!socket)
    {
        printf("could not connect to server\n");
        exit(-1);
    }

    return socket;
}

Connection::~Connection()
{
    sendPackage(LOGOUT);
    SDLNet_TCP_Close(this->sock);
}

void Connection::getPlayers(void)
{
    printf("requesting player list..\n");

    sendPackage(REQ_PLAYERLIST);

    uint32_t nplayers;
    recievePackage(&nplayers);

    printf("%d players logged in\n", nplayers);
    for(int i=0; i < nplayers; i++)
    {
        LoginPackage p;
        recievePackage(&p);
        this->playerlist.push_back(p);

        printf("* %s\n", p.name);
    }

    sendPackage(OK);
}
