
#pragma once

#include <SDL2/SDL_net.h>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include <server/packages.h>

#include <math/vector.h>

using namespace huge;

class Connection : public Socket
{
    public:
        Connection(char const* const servername, uint16_t serverPort, char const* const username);
        ~Connection();

        int handle();

        bool reqAction();
        void action(uint32_t game_id, Vector3i lvl1, Vector3i lvl2);

        int reqGame(std::string name);
        int reqGame(uint32_t player_id);
        void getPlayers(void);

        std::vector<std::pair<Vector3i,Vector3i>> actions;
        std::map<std::string, uint32_t> playerlist;

    private:
        SDLNet_SocketSet set;
        uint32_t id;
        bool reqaction;
};
