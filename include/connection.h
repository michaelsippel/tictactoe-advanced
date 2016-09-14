
#pragma once

#include <SDL2/SDL_net.h>
#include <cstdint>
#include <vector>

#include <server/packages.h>

class Connection
{
    public:
        Connection(char const* const servername, uint16_t serverPort, char const* const username);
        ~Connection();

        void getPlayers(void);

        void sendAction(ActionPackage pos);
        ActionPackage recieveAction(void);

        std::vector<LoginPackage> playerlist;
    private:
        IPaddress server_address;
        TCPsocket server;
        SDLNet_SocketSet set;
        unsigned int id;
};
