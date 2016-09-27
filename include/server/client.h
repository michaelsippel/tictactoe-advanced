#pragma once

#include <map>
#include <server/packages.h>

class Game;

class Client : public Socket
{
    public:
        Client(TCPsocket sock_);
        ~Client();

        void handle(std::map<uint32_t, Client*>::iterator it, std::map<uint32_t, Client*>& playerlist,
                    std::map<uint32_t, Game*>& gamelist, SDLNet_SocketSet& set);
        char* name;

    private:
        // actions
        void logout(std::map<uint32_t, Client*>::iterator it, std::map<uint32_t, Client*>& playerlist,
                    SDLNet_SocketSet& set);
};
