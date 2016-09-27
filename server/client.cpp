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

                std::map<uint32_t, Client*>::iterator it = playerlist.find(player_id);
                if(it != playerlist.end())
                {
                    Client* player = it->second;
                    printf("[server] ask %s about a game\n", player->name);

                    player->sendPackage(REQ_GAME);
                    player->sendPackage(it->first); // this id

                    // redirect response
                    connSignal sig;
                    player->recievePackage(&sig);
                    this->sendPackage(sig);
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
