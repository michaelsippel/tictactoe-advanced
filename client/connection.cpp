
#include <cstdint>
#include <map>
#include <string>
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
                        return 1;
                    }
                    else
                        this->sendPackage(NO);
                }
                break;
            }
        }
    }
    return 0;
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

    sendPackage(OK);

    return 0;
}

void Connection::getPlayers(void)
{
    printf("requesting player list..\n");

    sendPackage(REQ_PLAYERLIST);

    uint32_t nplayers;
    recievePackage(&nplayers);

    printf("%d players logged in\n", nplayers);
    for(int i=0; i < nplayers; i++)
    {
        uint32_t id;
        this->recievePackage(&id);
        char* name = this->recieveString();

        this->playerlist.insert(std::pair<std::string, uint32_t>(name, id));
        printf("* %s\n", name);
    }

    sendPackage(OK);
}
