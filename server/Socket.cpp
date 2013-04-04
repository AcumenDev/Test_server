#include "Socket.h"

Socket::Socket()
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData = {0};
    wVersionRequested = MAKEWORD(2, 2);
    _ErrorCode = WSAStartup(wVersionRequested, &wsaData);
    _Socket =  socket(AF_INET,SOCK_STREAM, 0);
}

Socket::Socket(SOCKET socket)
{
    _Socket =  socket;
}

Socket::~Socket()
{
    WSACleanup();
}

int Socket::Bind(int port,std::string adress)
{
    sockaddr_in service;
    service.sin_family = AF_INET;
    if(adress.empty())
        service.sin_addr.s_addr = INADDR_ANY;// inet_addr("127.0.0.1");
    else
        service.sin_addr.s_addr = inet_addr(adress.c_str());

    service.sin_port = htons(port);


    _ErrorCode = bind(_Socket, (SOCKADDR *) &service, sizeof (service));
//    if (_ErrorCode == SOCKET_ERROR)
//    {
//        wprintf(L"bind failed with error %u\n", WSAGetLastError());
//        closesocket(_Socket);
//        WSACleanup();
//        return 1;
//    }
    return 0 ;
}

int Socket::listen()
{
    _ErrorCode =::listen(_Socket, 0x100);

    return 0 ;
}

int Socket::Connect(std::string adress, int port)
{
    sockaddr_in dest_addr;
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(port);
    HOSTENT *hst;
// преобразование IP адреса из символьного в сетевой формат
    if (inet_addr(adress.c_str())!=INADDR_NONE)
        dest_addr.sin_addr.s_addr=inet_addr(adress.c_str());
    else
    {


// попытка получить IP адрес по доменному имени сервера
        hst=gethostbyname(adress.c_str());
        if (hst == nullptr)
// hst>h_addr_list содержит не массив адресов,
// а массив указателей на адреса
            ((unsigned long *)&dest_addr.sin_addr)[0]=((unsigned long **)hst->h_addr_list)[0][0];
        else
        {
            std::cout<<"Invalid address \n " << adress;
            //  closesocket(my_sock);
            // WSACleanup();
            return 0;
        }
    }
    // адрес сервера получен – пытаемся установить соединение
    if (connect(_Socket,(sockaddr *)&dest_addr,sizeof(dest_addr)))
    {
        std::cout<<"Connect error \n "<< WSAGetLastError();
        return 0;
    }
    std::cout<<"Connect to %s successfully established \n Type quit for quit\n\n",adress.c_str();
    return 0 ;
}

Socket * Socket::Accept()
{
    SOCKET client_socket; // сокет для клиента
    sockaddr_in client_addr; // адрес клиента (заполняется системой)

    int client_addr_size=sizeof(client_addr);

    client_socket=accept(_Socket, (sockaddr *) &client_addr, &client_addr_size);

    HOSTENT *hst;
    hst=gethostbyaddr((char *) &client_addr.sin_addr.s_addr,4,AF_INET);

    std::string nameClient = (hst !=nullptr)?hst->h_name:"";
    std::cout<<"New connect!\n"<< nameClient <<": "<<inet_ntoa(client_addr.sin_addr);

    // Socket * resultSocket =  new Socket(client_socket);

    return new Socket(client_socket);;
}

char * Socket::Recv()
{
    // char * buff[1024];
    char * buff = new char[1024];

    int  nsize=recv(_Socket,buff,sizeof(buff),0);
    if(nsize !=SOCKET_ERROR)
    {
// ставим завершающий ноль в конце строки
        buff[nsize]=0;
// выводим на экран
        std::cout<<buff<<std::endl;
        return buff;
    }

    return buff ;
}

int Socket::Send(char * data,int nsize )
{
// передаем строку клиента серверу
    return send(_Socket,data,nsize,0);
}

