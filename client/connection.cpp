
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
