/*
 * this server creates a thread for each incoming connection (TCP)
 * after the NUMBER_OF_THREADS connections server terminates
 *
 *  improvements compared to server.cc:
 *  - getaddrinfo
 *  - perror
 */

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>     
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

char client_message[1024];

const int NUMBER_OF_THREADS = 1;    // number of possible connections
pthread_t tid[NUMBER_OF_THREADS];

const int BACKLOG = 10;             // max size for the queue of pending connections
const char *PORT = "7799";
//const char* IP = "127.0.0.1";
const char *IP = "10.0.1.1";
const char *message = "MSG(server): got your message!";

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void *socketThread(void *arg) {

    int newSocket = *((int *)arg);

    if(recv(newSocket, client_message, 1024, 0) )
        printf("Server: data received: %s\n", client_message);
    else
        perror("Server: error: receive failed!");
   
    //pthread_mutex_lock(&lock);
    //pthread_mutex_unlock(&lock);

    printf("Server: sending response to client: %s\n", message);
    if( send(newSocket , message , strlen(message) , 0) < 0)
        perror("Server: error: send failed!");
    

    printf("Server: closing socket and thread \n");
    close(newSocket);
    pthread_exit(NULL);
}


int main(){

    int serverSocket;
    int newSocket;
    socklen_t addr_size;
    struct sockaddr_storage serverAddr;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    //hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    getaddrinfo(IP, PORT, &hints, &res);

    serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(serverSocket, res->ai_addr, res->ai_addrlen);

    if ( listen(serverSocket, BACKLOG) == 0 )
        printf("Server: Listening at %s:%s... \n", IP, PORT);
    else
        perror("Server: Error while trying to listen on a socket!");

    sleep(10);

    for (int i=0; i<NUMBER_OF_THREADS; i++){
   
        addr_size = sizeof serverAddr;

        // accept call creates a new socket for the incoming connection
        newSocket = accept(serverSocket, (struct sockaddr *) &serverAddr, &addr_size);

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
