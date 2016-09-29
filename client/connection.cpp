
#include <cstdint>
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <SDL2/SDL_net.h>
#include <server/packages.h>

#include <connection.h>

Connection::Connection(char const* const servername, uint16_t serverPort, char const* const username)
    : Socket(servername, serverPort)
{
    this->sendString(username);

    connSignal sig;
    recievePackage(&sig);
    if(sig == ID)
    {
        recievePackage(&this->id);
        sendPackage(OK);

        this->set = SDLNet_AllocSocketSet(1);
        if(SDLNet_TCP_AddSocket(this->set, this->sock) == -1)
        {
            printf("Unable to add server-socket to sockset\n");
            exit(-1);
        }
    }
}

Connection::~Connection()
{
    sendPackage(LOGOUT);
    SDLNet_TCP_Close(this->sock);
}

bool Connection::reqAction()
{
    return this->reqaction;
}

void Connection::action(uint32_t game_id, Vector3i lvl1, Vector3i lvl2)
{
    this->sendPackage(ACTION);
    this->sendPackage(game_id);
    this->sendPackage(std::pair<Vector3i,Vector3i>(lvl1, lvl2));

    connSignal sig;
    this->recievePackage(&sig);
    if(sig == OK)
        this->reqaction = false;
    else
        printf("Error trying to send move\n");

    this->sendPackage(OK);
}

int Connection::handle()
{
    if(SDLNet_CheckSockets(this->set, 1))
    {
        if(this->ready())
        {
            connSignal sig;
            recievePackage(&sig);
            switch(sig)
            {
                case REQ_GAME:
                {
                    uint32_t player_id;
                    this->recievePackage(&player_id);

                    char res;

                    std::cout << "Player " << player_id << " wants to start a new game. Accept? (y/n) ";
                    std::cin >> res;

                    if(res == 'y' || res == 'Y')
                    {
                        this->sendPackage(OK);

                        uint32_t game_id;
                        this->recievePackage(&game_id);
                        return game_id;
                    }
                    else
                        this->sendPackage(NO);
                }
                break;

                case REQ_ACTION:
                    this->reqaction = true;
                    break;

                case ACTION:
                {
                    uint32_t game_id;
                    std::pair<Vector3i,Vector3i> a;
                    this->recievePackage(&game_id);
                    this->recievePackage(&a);

                    this->actions.push_back(a);
                }
                break;
            }
        }
    }
    return -1;
}

int Connection::reqGame(std::string name)
{
    uint32_t id = this->playerlist.find(name)->second;
    return this->reqGame(id);
}

int Connection::reqGame(uint32_t player_id)
{
    sendPackage(REQ_GAME);
    sendPackage(player_id);

    connSignal sig;
    recievePackage(&sig);
    if(sig != OK)
    {
        if(sig == NO)
            printf("player dosn't accept a new game.\n");
        else
            printf("error requesting new game\n");
        return -1;
    }

    uint32_t game_id;
    this->recievePackage(&game_id);

    sendPackage(OK);

    return game_id;
}

void Connection::getPlayers(void)
{
    this->playerlist.clear();

    sendPackage(REQ_PLAYERLIST);

    uint32_t nplayers;
    recievePackage(&nplayers);

    for(int i=0; i < nplayers; i++)
    {
        uint32_t id;
        this->recievePackage(&id);
        char* name = this->recieveString();

        this->playerlist.insert(std::pair<std::string, uint32_t>(name, id));
    }

    sendPackage(OK);
}
