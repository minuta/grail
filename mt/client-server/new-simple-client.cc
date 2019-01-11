/*
 *  this client sends hello to server and tries to receive a response from server over a TCP connection
 *
 *  simple, one threaded client
 *  improvements compared to simple-client.cc:
 *  - getaddrinfo
 *  - perror 
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h> 

#include <netdb.h>
#include <stdlib.h>


//const char *IP = "127.0.0.1";
const char *IP = "10.0.1.1";
const char *PORT = "7799";

int main(){

    int status;
    struct addrinfo hints, *res;
    char buffer[1024];
    int clientSocket;

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    if ((status = getaddrinfo(IP, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    clientSocket = socket( res->ai_family, res->ai_socktype, res->ai_protocol );

    printf("Client: connecting to server...%s:%s\n", IP, PORT);

    if (connect(clientSocket, res->ai_addr, res->ai_addrlen)!=0)
        perror("Client: error: connection failed, status-code");
    
    char msg[] = "MSG(client): hello from client";

    printf("Client: sending request to server: %s\n", msg);
    if( send(clientSocket , msg , strlen(msg) , 0) < 0)
        perror("Client: error: send failed!");
    

    if(recv(clientSocket, buffer, 1024, 0) )
        printf("Client: data received: %s\n", buffer);
    else
        perror("Client: error: receive failed!");
   
    close(clientSocket);
    freeaddrinfo(res); // free the linked-list
    printf("Client: terminating...\n");

    return 0;
}
