
#pragma once

#include <SDL2/SDL_net.h>
#include <cstdlib>
#include <cstring>

class Socket
{

    public:
        Socket(TCPsocket sock_)
            : sock(sock_)
        {}

        Socket(char const* const servername, uint16_t serverPort)
        {
            IPaddress server_address;
            SDLNet_ResolveHost(&server_address, servername, serverPort);
            this->sock = SDLNet_TCP_Open(&server_address);
            if(!this->sock)
            {
                printf("could not connect to server\n");
                exit(-1);
            }
        }

        template<typename DataType>
        int recievePackage(DataType* const data)
        {
            if(SDLNet_TCP_Recv(this->sock, (void*)data, sizeof(DataType)) < sizeof(DataType))
            {
                printf("couldn't recieve package! %s\n", SDLNet_GetError());
                return -1;
            }

            return 0;
        }

        template<typename DataType>
        int sendPackage(DataType const& data)
        {
            if(SDLNet_TCP_Send(this->sock, (void*)&data, sizeof(DataType)) < sizeof(DataType))
            {
                printf("couldn't send package! %s\n", SDLNet_GetError());
                return -1;
            }

            return 0;
        }

        void sendString(char const* name)
        {
            /*            uint32_t len = strlen(name);
                        sendPackage(len);

                        if(SDLNet_TCP_Send(this->sock, (void*)name, len) < len);
                        {
                            printf("couldn't send string! %s\n", SDLNet_GetError());
                        }*/
            char buf[512];
            strcpy(buf, name);
            sendPackage(buf);
        }

        char* recieveString(void)
        {
            /*
               uint32_t len;
               recievePackage(&len);
               char* name = (char*) malloc(len+1);
               name[len] = '\0';
               if(SDLNet_TCP_Recv(this->sock, (void*)&str, len) < len);
               {
                   printf("couldn't recieve string! %s\n", SDLNet_GetError());
                   return NULL;
               }
               return name;*/

            char buf[512];
            recievePackage(&buf);

            char* str = (char*) malloc(strlen(buf)+1);
            strcpy(str, buf);
            return str;
        }

        bool ready()
        {
            return SDLNet_SocketReady(this->sock);
        }

    protected:
        TCPsocket sock;
};

typedef enum
{
    NOSIGNAL,
    OK,
    NO,
    ERR,
    ID,
    LOGOUT,
    REQ_PLAYERLIST,
    REQ_GAME,
    ACTION,
    REQ_ACTION,
} connSignal;
