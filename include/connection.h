
#pragma once

#include <SDL2/SDL_net.h>
#include <cstdint>
#include <string>
#include <map>

#include <server/packages.h>

class Connection : public Socket
{
    public:
        Connection(char const* const servername, uint16_t serverPort, char const* const username);
        ~Connection();

        int handle();

        int reqGame(std::string name);
        int reqGame(uint32_t player_id);
        void getPlayers(void);

        std::map<std::string, uint32_t> playerlist;

    private:
        SDLNet_SocketSet set;
        uint32_t id;
};
