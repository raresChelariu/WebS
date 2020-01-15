#include "tools.h"

class ThreadAction
{
public:
    int cd;
    pthread_mutex_t* lock;
    ThreadAction(pthread_mutex_t* t, int cd);
    bool Login();
    void ClientCommand();
};
ThreadAction::ThreadAction(pthread_mutex_t* lock, int cd)
{
    this->lock = lock;
    this->cd = cd;
}
bool ThreadAction::Login()
{
    printf("Login function:\nCd is %d", this->cd); fflush(stdout);
    char *user, *pass, *lineUser, *linePass;
    char msg[256], line[256];
    memset(msg, 0, 256);

    read(cd, msg, 256);
    user = strtok(msg, ";");
    pass = strtok(NULL, ";");

    printf("11%s22%s22\n", user, pass); fflush(stdout);
    pthread_mutex_lock(lock);
    FILE* fileUserPass = fopen("userpass.txt", "r");
    int isLogged = 0;
    while (fgets(line, 256, fileUserPass))
    {
        *strchr(line, '\n') = 0;
        lineUser = strtok(line, ";");
        linePass = strtok(NULL, ";");
        if (!strcmp(lineUser, user) && !strcmp(linePass, pass))
        {   
            isLogged = 1;
            break;
        }            
    }
    strcpy(msg, isLogged ? CMD_LOGIN_GOOD : CMD_LOGIN_BAD);
    write(cd, msg, 256);
    pthread_mutex_unlock(lock);
    fclose(fileUserPass);
    return isLogged;
}
void ThreadAction::ClientCommand()
{
    char msg[256];
    if (0 == read(cd, msg, 256))
    {
        printf("Client disconnected!\n");
        fflush(stdout);
        pthread_cancel(pthread_self());
    }
    else if (!strcmp(msg, CMD_LIST_ALL))
    {
        DIR* dir;
        dirent* ent;
        printf("Listing all pages \n"); fflush(stdout);
        if (NULL != (dir = opendir("./storage")))
        {
            while (NULL != (ent = readdir(dir)))
            {
                if (ent->d_name[0] == '.' || strstr(ent->d_name, ".htm") == NULL)
                    continue;
                    write(cd, ent->d_name, 256);
                printf("%s\n", ent->d_name);
                fflush(stdout);
            }
        }
        else
        {
            printf("Error on accesing dir!\n");
            fflush(stdout);
        }
        write(cd, CMD_END, 256);
    }
    else if (!strcmp(msg, CMD_GET_PAGE))
    {
        char line[256], fileName[256], filePath[256];
        memset(line, 0, 256); 
        memset(fileName, 0, 256);
        memset(filePath, 0,256);

        read(cd, fileName, 256);
        printf("GETTING PAGE 111%s111\n", fileName);
        sprintf(filePath, "./storage/%s", fileName);

        FILE* selectedFile = fopen(filePath, "r");
        printf("GET OUTPUT : \n\n\n");
        while (fgets(line, 256, selectedFile))
        {
            write(cd, line, 256);
            printf("%s", line);
        }
        fflush(stdout);
        fclose(selectedFile);
        write(cd, CMD_END, 256);
    }
    else if (!strcmp(msg, CMD_ADD_PAGE))
    {
        char fileName[256], filePath[256], *text;
        int fileSize;
        memset(fileName, 0, 256);
        memset(fileName, 0, 256);

        read(cd, fileName, 256);
        printf("\n\n%s\n", fileName); fflush(stdout);
        read(cd, &fileSize, 4);
        printf("%d\n", fileSize); fflush(stdout);
        text = (char*) malloc(fileSize);
        memset(text, 0, fileSize);
        
        read(cd, text, fileSize);

        sprintf(filePath, "./storage/%s", fileName);
        FILE* newFile = fopen(filePath, "w");
        fprintf(newFile, "%s", text);
        printf("Page added successfully!\n");
        fflush(stdout);
    }
    else if (!strcmp(msg, CMD_DELETE_PAGE))
    {
        char fileName[256], filePath[256];
        memset(fileName, 0, 256);
        memset(filePath, 0, 256);

        read(cd, fileName, 256);
        sprintf(filePath, "./storage/%s", fileName);

        printf("Removing this file : %s\n", filePath);
        if (0 != remove(filePath))
        {
            perror("Error on removing file!\n");
            fflush(stdout);
        }
        else
        {
            printf("File removed successfully!\n");
            fflush(stdout);
        }
    }
    else
    {
        printf("Bad command! :( \n Client has requested : '%s'", msg);
        fflush(stdout);
    }
}