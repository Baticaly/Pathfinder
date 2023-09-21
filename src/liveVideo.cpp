#include "liveVideo.h"
#include "imx219.h"
#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void transmitImage(int serverPort)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket" << std::endl;
        return;
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "ERROR setting socket options" << std::endl;
        close(sockfd);
        return;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "ERROR binding to socket" << std::endl;
        close(sockfd);
        return;
    }

    listen(sockfd, 5);

    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);

    int newsockfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientLen);
    if (newsockfd < 0)
    {
        std::cerr << "ERROR accepting connection" << std::endl;
        close(sockfd);
        return;
    }

    while (true)
    {
        cv::Mat image;
        try {
            image = captureImage();
        } catch (const std::exception& e) {
            std::cerr << "ERROR capturing image: " << e.what() << std::endl;
            break;
        }

        std::vector<unsigned char> jpegData;
        try {
            jpegData = convertToJpeg(image);
        } catch (const std::exception& e) {
            std::cerr << "ERROR converting image to JPEG: " << e.what() << std::endl;
            break;
        }

        uint32_t size = htonl(jpegData.size()); // Ensure network byte order
        write(newsockfd, &size, sizeof(size));

        write(newsockfd, jpegData.data(), jpegData.size());

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    close(newsockfd);
    close(sockfd);
}