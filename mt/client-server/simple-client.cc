//
// this client sends hello to server and tries to receive a response from server
//
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 


const int PORT = 7799;
//const char * IP = "127.0.0.1";
const char * IP = "10.0.1.1";


int main(){

    char message[1000];
    char buffer[1024];
    int clientSocket;
    struct sockaddr_in serverAddr;

    socklen_t addr_size;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    addr_size = sizeof serverAddr;

    printf("Client: connecting to server...\n");
    int connection_status = connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    if (connection_status!=0){
        printf("Client: error: connection failed, status-code: %d", connection_status);
    }
    
    strcpy (message, "hello from client \n");

    if( send(clientSocket , message , strlen(message) , 0) < 0) {
            printf("Client: error: send failed! \n");
    }

    if(recv(clientSocket, buffer, 1024, 0) ) {
        printf("Client: data received: %s\n", buffer);
    } else {
        printf("Client: error: receive failed! \n");
    }

    close(clientSocket);
    
    printf("Client: terminating...\n");

    return 0;
}
