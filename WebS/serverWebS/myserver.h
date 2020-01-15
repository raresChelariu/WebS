#include "serverThread.h"
#include "tools.h"

#define PORT 3000
#define MAX_CONN_REQ_IN_QUEUE 30
#define MAX_THREAD_NO 100
class MyServer
{
    public:
        pthread_t threads[MAX_THREAD_NO];
        int threadID{0};
        sockaddr_in srvSock;
        int srvDesc;
        pthread_mutex_t lock;
    public:   
        MyServer();
        void ConfigServer();
        int ClientAccepted();
        void NewThread(int cd);
        static void ServerError();
};
void MyServer::NewThread(int cd)
{
    ServerThread * sTh = new ServerThread(&threads[threadID++], &lock, cd);
}
MyServer::MyServer()
{
    memset(&srvSock, 0, sizeof(sockaddr_in));
    memset(threads, 0, MAX_THREAD_NO*sizeof(pthread_t));

    srvSock.sin_family = AF_INET;
    srvSock.sin_port = htons(PORT);
    srvSock.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == pthread_mutex_init(&lock, NULL))
    {
        printf("Mutex initialisation has failed!\n");
        fflush(stdout);
    }
}
void MyServer::ConfigServer()
{
    if (-1 == (srvDesc = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("Error on making the sever socket!\n");
        exit(1);
    }
    int reuseSameAdress = 1;
    setsockopt(this->srvDesc, SOL_SOCKET, SO_REUSEADDR, &reuseSameAdress, 4);
    if (-1 == bind(this->srvDesc, (sockaddr*)&this->srvSock, sizeof(sockaddr)))
    {
        printf("Error - trying to bind failed!\n");
        exit(2);
    }
    if (-1 == listen(srvDesc, MAX_CONN_REQ_IN_QUEUE))
    {
        printf("Error on attempting to listen for clients!\n");
        exit(3);
    }
    printf("Server ready to listen for incoming new connection requests!\n");
    fflush(stdout);
}
int MyServer::ClientAccepted()
{
    sockaddr_in cliSock;
    socklen_t len = sizeof(sockaddr_in);
    memset(&cliSock, 0, sizeof(sockaddr_in));
    
    return accept(this->srvDesc, (sockaddr*)&cliSock, &len);
}