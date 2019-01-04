#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <pthread.h>


const int NUMBER_OF_THREADS = 5;  // number of possible connections
const int PORT = 7799;
const char* IP = "127.0.0.1";

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * clientThread(void *arg) {

    char message[1000];
    char buffer[1024];
    int clientSocket;
    struct sockaddr_in serverAddr;

    socklen_t addr_size;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    //pthread_mutex_lock(&lock);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    
    strcpy (message, "hello from client \n");

    if( send(clientSocket , message , strlen(message) , 0) < 0) {
            printf("Error: send failed! \n");
    }

    if(recv(clientSocket, buffer, 1024, 0) ) {
        printf("Data received: %s\n", buffer);
    } else {
        printf("Error: receive failed! \n");
    }

    //pthread_mutex_unlock(&lock);

    close(clientSocket);
    pthread_exit(NULL);
}


int main(){

    pthread_t tid[NUMBER_OF_THREADS];

    for (int i=0; i<NUMBER_OF_THREADS; i++){
        if( pthread_create(&tid[i], NULL, clientThread, NULL) != 0 )
            printf("Error: failed to create thread!\n");
    }

    sleep(10);

    for (int i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join ( tid[i], NULL);
    }
    
    return 0;
}
