#include "myserver.h"
#include <stdio.h>
#include <dirent.h>
int clientDesc;
char filePath[256];
char fileName[256];
char line[256];
int main()
{
    /*
    strcpy(fileName, "index.html");    
    sprintf(filePath, "./storage/%s", fileName);
    FILE* selectedFile = fopen(fileName, "r");
    while (fgets(line, 256, selectedFile))
    {
        write(cd, line, 256);
        printf("%s", line);
    }*/
    MyServer server;
    server.ConfigServer();
    while (1) 
    {
        clientDesc = server.ClientAccepted();
        if (-1 != clientDesc)
        {
            printf("New client accepted!\n");
            fflush(stdout);
            server.NewThread(clientDesc);
        }            
        else 
        {
            printf("Error on accepting new client to connect!\n");
            fflush(stdout);
        }
    }
    return 0;
}