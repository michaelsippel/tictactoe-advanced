
#pragma once

#include <SDL2/SDL_net.h>

template<typename DataType>
void recievePackage(TCPsocket client, DataType* data)
{
    if(SDLNet_TCP_Recv(client, (void*)data, sizeof(DataType)) != sizeof(DataType))
    {
        printf("couldn't recieve package!\n");
    }
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

} connSignal;

struct LoginPackage
{
    char name[64];
};
