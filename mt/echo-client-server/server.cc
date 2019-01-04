#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>     
#include <pthread.h>

char client_message[2000];

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void * socketThread(void *arg) {

    int newSocket = *((int *)arg);
    recv(newSocket , client_message , 2000 , 0);

    printf("got client message : %s", client_message);


    //pthread_mutex_lock(&lock);
    //pthread_mutex_unlock(&lock);

    sleep(1);

    char const * message = "Hello from server : got your message!";

    send(newSocket, message, strlen(message), 0);


    printf("Exit thread and its spawned thread \n");
    close(newSocket);
    pthread_exit(NULL);
}


int main(){

    struct sockaddr_in serverAddr;
    int serverSocket, newSocket;

    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    //Create the socket. 
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    // Configure settings of the server address struct
    // Address family = Internet 
    serverAddr.sin_family = AF_INET;

    //Set port number, using htons function to use proper byte order 
    serverAddr.sin_port = htons(7799);

    //Set IP address to localhost 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Set all bits of the padding field to 0 
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Bind the address struct to the socket 
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if ( listen(serverSocket, 5) == 0 )
        printf("Listening\n");
    else
        printf("Error\n");

    pthread_t tid[9];
    //int i = 0;

    for (int i=0; i<3; i++){
         //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        //for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
        if( pthread_create(&tid[i], NULL, socketThread, &newSocket) != 0 )
            printf("Failed to create thread\n");
    }


    for (int i=0; i<3; i++){
        pthread_join(tid[i], NULL);
    }

    return 0;
}
