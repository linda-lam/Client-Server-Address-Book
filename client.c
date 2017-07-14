#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    char buffer[256];
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    
    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr,"ERROR, host doesn't exist\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("Connection ERROR");
    }
    
    int repeat = 0;
    printf("%s\n", "Address server started");
    
    int n;
    
    while (repeat == 0)
    {
        
        printf("%s", "> ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        
        if (strcmp(buffer, "+++\n") == 0)
        {
            repeat = 1;
        }
        else
        {
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0)
            {
                error("ERROR");
            }
            bzero(buffer,256);
            n = read(sockfd,buffer,255);
            if (n < 0)
            {
                error("ERROR");
            }
            printf("%s\n",buffer);
        }
    }
    close(sockfd);
    return 0;
}
