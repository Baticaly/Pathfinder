#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mutex>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tcpSocketListener.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void tcpSocketListener(std::vector<int> &motorData, std::mutex &motorMutex)
{
    const int portno = 8080;
    int sockfd, newsockfd;
    socklen_t clilen;

    char buffer[256];
    struct sockaddr_in serverAddress, clientAddress;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        error("ERROR on binding");

    while (true)
    {
        listen(sockfd, 5);

        clilen = sizeof(clientAddress);
        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
        {
            error("ERROR reading from socket");
            motorMutex.lock();
            motorData.clear();
            motorMutex.unlock();
        }

        motorMutex.lock(); // Lock the mutex before modifying motorData

        motorData.resize(8); // Adjust the size of the motorData vector

        sscanf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d",
               &motorData[0], &motorData[1], &motorData[2], &motorData[3],
               &motorData[4], &motorData[5], &motorData[6], &motorData[7]);

        motorMutex.unlock(); // Unlock the mutex after modifying motorData

        n = write(newsockfd, buffer, 255);
        if (n < 0)
        {
            error("ERROR writing to socket");
            motorMutex.lock();
            motorData.clear();
            motorMutex.unlock();
        }

        close(newsockfd);
    }

    close(sockfd);
}
