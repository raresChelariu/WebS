#ifndef MYCLIENT_H
#define MYCLIENT_H
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define PORT 3000
#define CMD_LOGIN_GOOD "LOGIN_GOOD"
#define CMD_LOGIN_BAD "LOGIN_BAD"
#define CMD_GET_PAGE "GET_PAGE"
#define CMD_LIST_ALL "LIST_ALL"
#define CMD_ADD_PAGE "ADD_PAGE"
#define CMD_DELETE_PAGE "DELETE_PAGE"
#define CMD_END "CMD_END"

class MyClient
{
public:
    static int cd;
    sockaddr_in serverSock;
    MyClient();
    bool connectToServer();
};

#endif // MYCLIENT_H
