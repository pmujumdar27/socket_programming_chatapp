#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc<2){
        fprintf(stderr, "Port number not provided. Program Terminated\n");
        exit(EXIT_FAILURE);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        error("Error opening socket.\n");
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0 ){
        error("Binding Failed\n");
    }

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

    if(newsockfd < 0){
        error("Error on accept\n");
    }

    while(1){
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if(n<0){
            error("Error on reading\n");
        }
        printf("Client: %s", buffer);
        bzero(buffer, 255);

        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));
        if(n<0){
            error("Error on writing\n");
        }
        if(strncmp("bye", buffer, 3)==0){
            break;
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}