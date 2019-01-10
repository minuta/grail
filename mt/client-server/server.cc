//
// this server creates a thread for each incoming connection
// after the NUMBER_OF_THREADS connections server terminates
//
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>     
#include <pthread.h>

char client_message[2000];
const int NUMBER_OF_THREADS = 1;    // number of possible connections
const int BACKLOG = 10;             // max size for the queue of pending connections
const int PORT = 7799;
//const char* IP = "127.0.0.1";
const char *IP = "10.0.1.1";
const char *message = "Hello from server : got your message!";

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void *socketThread(void *arg) {

    int newSocket = *((int *)arg);
    recv(newSocket , client_message , 2000 , 0);

    printf("Server: got client message : %s \n", client_message);

    //pthread_mutex_lock(&lock);
    //pthread_mutex_unlock(&lock);

    send(newSocket, message, strlen(message), 0);

    printf("Server: close socket and thread \n");
    close(newSocket);
    pthread_exit(NULL);
}


int main(){

    struct sockaddr_in serverAddr;
    int serverSocket, newSocket;

    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;

    serverAddr.sin_port = htons(PORT);

    serverAddr.sin_addr.s_addr = inet_addr(IP);

    //Set all bits of the padding field to 0 
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if ( listen(serverSocket, BACKLOG) == 0 )
        printf("Server: Listening... \n");
    else
        printf("Server: Error while trying to listen on a socket! \n");

    pthread_t tid[NUMBER_OF_THREADS];

    sleep(10);

    for (int i=0; i<NUMBER_OF_THREADS; i++){
   
        // accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;

        //printf("Server: before syscall accept \n");
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        //printf("Server: after syscall accept \n");
        
        //for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
        if ( pthread_create(&tid[i], NULL, socketThread, &newSocket) != 0 )
            printf("Server: error: failed to create thread!\n");
    }

    sleep(5);
    for (int i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join ( tid[i], NULL);
    }
    
    printf("Server: terminating...\n");

    return 0;
}
