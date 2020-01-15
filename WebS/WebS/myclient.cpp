#include "myclient.h"
#define MYHOST "127.0.0.1"
#include <string.h>

int MyClient::cd;
MyClient::MyClient()
{
    memset(&serverSock, 0, sizeof(sockaddr_in));
    serverSock.sin_family = AF_INET;
    serverSock.sin_port = htons(PORT);
    serverSock.sin_addr.s_addr = inet_addr(MYHOST);
}
bool MyClient::connectToServer()
{
    if (-1 == (MyClient::cd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("Error on creating socket!\n");
        return 0;
    }
    if (-1 == ::connect(MyClient::cd, (sockaddr*)&serverSock, sizeof(sockaddr)))
    {
        printf("Error on trying to connect to server!\n");
        return 0;
    }
    printf("Connection was made!");
    fflush(stdout);
    return 1;
}
