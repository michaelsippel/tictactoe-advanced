#include <set>
#include <cstdlib>
#include <server/client.h>
#include <server/packages.h>
#include <server/game.h>

Client::Client(TCPsocket sock_)
    : Socket(sock_)
{
    this->name = this->recieveString();
}

void Client::logout(std::map<uint32_t, Client*>::iterator it, std::map<uint32_t, Client*>& playerlist,
                    SDLNet_SocketSet& set)
{
    playerlist.erase(it);
    SDLNet_TCP_AddSocket(set, this->sock);
    SDLNet_TCP_Close(this->sock);
}

Client::~Client()
{
    free(this->name);
}

void Client::handle(std::map<uint32_t, Client*>::iterator it, std::map<uint32_t, Client*>& playerlist,
                    std::map<uint32_t, Game*>& gamelist, SDLNet_SocketSet& set)
{
    if(this->ready())
    {
        connSignal sig;
        recievePackage(&sig);
        switch(sig)
        {
            case LOGOUT:
                this->logout(it, playerlist, set);
                printf("[server] %s logged off\n", this->name);
                delete this;
                return;

            case REQ_PLAYERLIST:
                sendPackage((uint32_t) playerlist.size());
                for(auto c : playerlist)
                {
                    this->sendPackage(c.first); // id
                    this->sendString(c.second->name); // name
                }
                break;

            case REQ_GAME:
            {
                uint32_t player_id;
                recievePackage(&player_id);

                std::map<uint32_t, Client*>::iterator pit = playerlist.find(player_id);
                if(pit != playerlist.end())
                {
                    Client* player = pit->second;
                    printf("[server] ask %s about a game\n", player->name);

                    player->sendPackage(REQ_GAME);
                    player->sendPackage(it->first); // this id

                    // redirect response
                    connSignal sig;
                    player->recievePackage(&sig);
                    this->sendPackage(sig);

                    if(sig == OK)
                    {
                        //  create new game
                        static uint32_t game_id_counter = 1;
                        Game* game = new Game(it->first, pit->first);
                        printf("created game\n");

                        uint32_t gid = game_id_counter++;
                        gamelist.insert(std::pair<uint32_t, Game*>(gid, game));
                        printf("insert in list\n");

                        this->sendPackage(gid);
                        player->sendPackage(gid);

                        // ask the first player to move
                        player->sendPackage(REQ_ACTION);
                    }
                }
                else
                    this->sendPackage(ERR);
            }
            break;

            case ACTION:
            {
                uint32_t game_id;
                this->recievePackage(&game_id);

                std::map<uint32_t, Game*>::iterator git = gamelist.find(game_id);
                if(git != gamelist.end())
                {
                    Game* game = git->second;

                    if(it->first == game->next_player_id())
                    {
                        this->sendPackage(OK);

                        std::pair<Vector3i, Vector3i> move;
                        this->recievePackage(&move);

                        game->move(move);

                        printf("[server] %s sets at (%d,%d,%d),(%d,%d,%d)\n", it->second->name,
                               move.first.x(), move.first.y(), move.first.z(),
                               move.second.x(), move.second.y(), move.second.z());

                        Client* player = playerlist.find(game->next_player_id())->second;
                        player->sendPackage(ACTION);
                        player->sendPackage(game_id);
                        player->sendPackage(move);
                        player->sendPackage(REQ_ACTION);
                    }
                    else
                        this->sendPackage(ERR);
                }
                else
                    this->sendPackage(ERR);
            }
            break;
        }

        recievePackage(&sig);
        if(sig != OK)
        {
            printf("[server] client %s doesn't respond.. kick him!\n", this->name);
            this->logout(it, playerlist, set);
            delete this;
            return;
        }
    }
}
