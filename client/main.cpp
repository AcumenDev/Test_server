#include <iostream>
#include "../server/Socket.h"
using namespace std;

int main()
{
   Socket* socketServer = new Socket();
   socketServer->Connect("127.0.0.1",9999);

    return 0;
}
