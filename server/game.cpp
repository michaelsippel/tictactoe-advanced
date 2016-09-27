
#include <server/game.h>
#include <server/client.h>

Game::Game()
{
    this->field = new Field3D(Vector3i(3,3,3),
                              [] ()
    {
        return new Field<3, CellStatus>(Vector3i(3,3,3),
                                        []()
        {
            return new CellStatus();
        });
    });
}

void Game::handle()
{

}
