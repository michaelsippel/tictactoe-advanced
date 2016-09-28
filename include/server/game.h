#pragma once

#include <cstdint>
#include <vector>
#include <math/vector.h>

class Client;

using namespace huge;

class Game
{
    public:
        Game(uint32_t id0, uint32_t id1);

        uint32_t next_player_id();
        uint32_t move(std::pair<Vector3i, Vector3i> m);

        // TODO: load & save

    private:
        std::vector< std::pair<Vector3i, Vector3i> > moves[2];
        uint32_t player_ids[2];
        int next;
};
