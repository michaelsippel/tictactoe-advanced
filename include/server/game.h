#pragma once

#include <field.h>

class Client;

class Game
{
    public:
        Game();
        ~Game();

        void handle();

    private:
        uint32_t player_ids[2];
        Client* players[2];
        Field3D* field;
};
