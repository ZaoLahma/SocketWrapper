/*
 * main.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: janne
 */

#include "../inc/SocketAPI.h"

int main(void) {

	SocketAPI socket;

	int serverSocket = socket.getServerSocketFileDescriptor("3370");

	int clientSocket = socket.waitForConnection(serverSocket);

	SocketBuf data = socket.receive_data(clientSocket, 6);

	for(unsigned int i = 0; i < data.dataSize; ++i)
	{
		printf("Byte %d: %c\n", i, data.data[i]);
	}

	socket.sendData(clientSocket, data);

	socket.receive_data(clientSocket, 1);

	delete data.data;

	return 0;
}
