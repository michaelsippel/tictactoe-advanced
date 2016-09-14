
#pragma once

#include <SDL2/SDL_net.h>

template<typename DataType>
int recievePackage(TCPsocket client, DataType* data)
{
    if(SDLNet_TCP_Recv(client, (void*)data, sizeof(DataType)) != sizeof(DataType))
    {
        printf("couldn't recieve package!\n");
        return -1;
    }

    return 0;
}

template<typename DataType>
int sendPackage(TCPsocket client, DataType* data)
{
    if(SDLNet_TCP_Send(client, (void*)data, sizeof(DataType)) != sizeof(DataType))
    {
        printf("couldn't send package!\n");
        return -1;
    }
    return 0;
}

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
