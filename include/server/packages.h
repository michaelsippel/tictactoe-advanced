
#pragma once

#include <SDL2/SDL_net.h>

class Socket
{

    public:
        Socket(TCPsocket sock_)
            : sock(sock_)
        {}

        template<typename DataType>
        int recievePackage(DataType* const data)
        {
            if(SDLNet_TCP_Recv(this->sock, (void*)data, sizeof(DataType)) != sizeof(DataType))
            {
                printf("couldn't recieve package!\n");
                return -1;
            }

            return 0;
        }

        template<typename DataType>
        int sendPackage(DataType const& data)
        {
            if(SDLNet_TCP_Send(this->sock, (void*)&data, sizeof(DataType)) != sizeof(DataType))
            {
                printf("couldn't send package!\n");
                return -1;
            }

            return 0;
        }

    protected:
        TCPsocket sock;
};

typedef enum
{
    NOSIGNAL,
    OK,
    ID,
    LOGOUT,
    REQ_PLAYERLIST,
    REQ_GAME,
    ACTION,
} connSignal;

struct LoginPackage
{
    uint32_t id;
    char name[64];
};

struct ActionPackage
{

};
