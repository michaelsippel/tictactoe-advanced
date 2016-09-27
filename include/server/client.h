#pragma once

#include <set>
#include <server/packages.h>

class Client : public Socket
{
    public:
        Client(TCPsocket sock_);
        void logout(std::set<Client*>& playerlist, SDLNet_SocketSet& set);

        char name[64];

        unsigned int id;
        static unsigned int id_counter;

        LoginPackage genPackagage();

        void handle(std::set<Client*>& playerlist, SDLNet_SocketSet& set);
};
