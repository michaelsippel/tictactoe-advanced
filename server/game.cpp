
#include <server/game.h>
#include <server/client.h>

Game::Game(uint32_t id0, uint32_t id1)
{
    this->player_ids[0] = id0;
    this->player_ids[1] = id1;

    this->next = 0;
}

uint32_t Game::next_player_id()
{
    return this->next;
}

uint32_t Game::move(std::pair<Vector3i, Vector3i> m)
{
    this->moves[this->next_player_id()].push_back(m);
    this->next = (this->next+1)%2;
}
