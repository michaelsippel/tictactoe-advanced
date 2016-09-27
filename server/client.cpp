#include <set>
#include <server/client.h>
#include <server/packages.h>

unsigned int Client::id_counter = 0;

Client::Client(TCPsocket sock_)
    : Socket(sock_)
{
    LoginPackage login;
    recievePackage(&login);

    strcpy(this->name, login.name);
    id = id_counter++;
}

void Client::logout(std::set<Client*>& playerlist, SDLNet_SocketSet& set)
{
    playerlist.erase(this);
    SDLNet_TCP_AddSocket(set, this->sock);
    SDLNet_TCP_Close(this->sock);
}

LoginPackage Client::genPackagage()
{
    LoginPackage p;
    strcpy(p.name, this->name);
    p.id = this->id;
    return p;
}

void Client::handle(std::set<Client*>& playerlist, SDLNet_SocketSet& set)
{
    connSignal sig;
    if(SDLNet_SocketReady(this->sock))
    {
        recievePackage(&sig);
        switch(sig)
        {
            case LOGOUT:
                this->logout(playerlist, set);
                printf("[server] %s logged off\n", this->name);
                delete this;
                return;

            case REQ_PLAYERLIST:
                sendPackage((uint32_t)playerlist.size());
                for(Client* c : playerlist)
                    sendPackage(c->genPackagage());
                break;
        }

        recievePackage(&sig);
        if(sig != OK)
        {
            printf("[server] client %s doesn't respond.. kick him!\n", this->name);
            this->logout(playerlist, set);
            delete this;
            return;
        }
    }
}
