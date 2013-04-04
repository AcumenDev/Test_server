#ifndef SOCKET_H
#define SOCKET_H
#include <iostream>
#include <winsock2.h>

class Socket
{
public:
    Socket();
    Socket(SOCKET socket);
    virtual ~Socket();
    int Bind(int port, std::string adress="");
    int listen();
    int Connect(std::string adress, int port);
    Socket* Accept();
    char * Recv();
    int Send(char * data,int nsize);
protected:
private:
    int _ErrorCode;
    SOCKET _Socket;
};

#endif // SOCKET_H
