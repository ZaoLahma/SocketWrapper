/*
 * SocketAPI.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: janne
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "../inc/socket_api.h"

int SocketAPI::getServerSocketFileDescriptor(const std::string& portNo) {

    int sockfd;
    struct addrinfo hints;
	struct addrinfo* servinfo;
    struct addrinfo* p;
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, portNo.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }


    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, 10) == -1) {
        perror("listen");
        exit(1);
    }

    return sockfd;
}

int SocketAPI::waitForConnection(int socketFileDescriptor) {
	struct sockaddr_storage their_addr;
    socklen_t sin_size;

    printf("server: waiting for connections...\n");

	sin_size = sizeof their_addr;
	int new_fd = accept(socketFileDescriptor, (struct sockaddr *)&their_addr, &sin_size);

	printf("server: got new connection...\n");
    return new_fd;
}

int SocketAPI::getClientSocketFileDescriptor(const std::string& address, const std::string& portNo) {
    int sockfd;

    struct addrinfo hints;
	struct addrinfo* servinfo;
	struct addrinfo* p;
    int rv;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(address.c_str(), portNo.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (::connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    return sockfd;
}

SocketBuf SocketAPI::receiveData(int fileDescriptor, int noOfBytes, bool expectAllBytes) {
	SocketBuf retVal;
	retVal.data = new char[noOfBytes];
	retVal.dataSize = 0;

    int numbytes = 0;

    while(noOfBytes > 0) {
		if ((numbytes = recv(fileDescriptor, &retVal.data[numbytes], noOfBytes, 0)) == -1) {
			perror("recv");
			exit(1);
		}

		noOfBytes -= numbytes;
		retVal.dataSize += numbytes;

		if(false == expectAllBytes) {
			break;
		}
    }

	return retVal;
}

int SocketAPI::sendData(int fileDescriptor, const SocketBuf& socketBuf) {

	int numBytesToSend = socketBuf.dataSize;
	int bytesSent = 0;

	while(numBytesToSend > 0) {
		bytesSent = send(fileDescriptor, socketBuf.data, socketBuf.dataSize, 0);
		if (bytesSent == -1) {
			perror("send");
			return -1;
		}

		numBytesToSend -= bytesSent;
	}

	return -1;
}

void SocketAPI::disconnect(int fileDescriptor)
{
	close(fileDescriptor);
}
