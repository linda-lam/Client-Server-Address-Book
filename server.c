#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    bzero(buffer,256);
    
    int repeat = 0;
    
    while (repeat == 0)
    {
        n = read(newsockfd,buffer,255);
        
        if (strcmp(buffer, "harris@ics.uci.edu\n") == 0)
        {
            n = write(newsockfd, "Ian G. Harris", 18);
            memset(buffer, 0, sizeof(buffer));
        }
        else if (strcmp(buffer, "joe@cnn.com\n") == 0)
        {
            n = write(newsockfd, "Joe Smith", 18);
            memset(buffer, 0, sizeof(buffer));
        }
        else if (strcmp(buffer, "jane@slashdot.org\n") == 0)
        {
            n = write(newsockfd, "Jane Smith", 18);
            memset(buffer, 0, sizeof(buffer));
        }
        else if (strcmp(buffer, "+++\n") == 0)
        {
            repeat = 1;
            n = write(newsockfd, "Address server ended", 18);
        }
        else
        {
            n = write(newsockfd, "unknown", 18);
        }
    }
    
    
    close(newsockfd);
    close(sockfd);
    return 0;
}
