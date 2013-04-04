#include <iostream>
#include "Socket.h"
using namespace std;

int main()
{
    cout << "Hello world!" << endl;
Socket* socketServer = new Socket();

socketServer->Bind(9999);
socketServer->listen();
socketServer->Accept();



    return 0;
}
