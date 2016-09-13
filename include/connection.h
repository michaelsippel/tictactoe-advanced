
#pragma once

#include <SDL2/SDL_net.h>
#include <cstdint>

class Connection
{
    public:
        Connection(char const* const servername, uint16_t serverPort, char const* const username);

    private:
        IPaddress server_address;
        TCPsocket server;
        SDLNet_SocketSet set;
        unsigned int id;
};
