
#pragma once

#include <SDL2/SDL_net.h>
#include <cstdint>
#include <vector>

#include <server/packages.h>

class Connection : public Socket
{
    public:
        Connection(char const* const servername, uint16_t serverPort, char const* const username);
        ~Connection();

        void getPlayers(void);

        void sendAction(ActionPackage pos);
        ActionPackage recieveAction(void);

        std::vector<LoginPackage> playerlist;

    private:
        static TCPsocket setupSocket(char const* const servername, uint16_t serverPort);

        SDLNet_SocketSet set;
        unsigned int id;
};
