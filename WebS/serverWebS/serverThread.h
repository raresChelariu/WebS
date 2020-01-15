#include "threadAction.h"


class ServerThread
{
public:
    struct ThreadItem
    {
        pthread_mutex_t* lock;
        int cd;
    };
    ServerThread(pthread_t* thread, pthread_mutex_t* lock, int cd);
    static void* ThreadRoutine(void* args);
};
ServerThread::ServerThread(pthread_t* thread, pthread_mutex_t* lock, int cd)
{
    ThreadItem* element = (ThreadItem*) malloc(sizeof(ThreadItem));
    element->lock = lock;
    element->cd = cd;
    pthread_create(thread, NULL, ThreadRoutine, (void*)element);
}
    
void* ServerThread::ThreadRoutine(void* args)
{
    printf("Thread created successfully!\n");
    fflush(stdout);
    ThreadItem currItem = *((ThreadItem*)args);
    printf("CD is %d!", currItem.cd); fflush(stdout);
    ThreadAction currThread(currItem.lock, currItem.cd);   

    int isLogged = 0;
    while (0 == isLogged)
        isLogged = currThread.Login();
    while (true)
    {
        currThread.ClientCommand();
    }
}