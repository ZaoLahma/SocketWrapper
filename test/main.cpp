/*
 * main.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: janne
 */

#include "../inc/SocketAPI.h"
#include <string>

int main(void) {

	SocketAPI socket;

	int serverSocket = socket.getServerSocketFileDescriptor("3370");

	int clientSocket = socket.waitForConnection(serverSocket);

	std::string hello_string = "220 hello, janne's ftp\r\n";

	SocketBuf send_data;
	send_data.dataSize = strlen(hello_string.c_str());
	send_data.data = new char[send_data.dataSize];

	memcpy(send_data.data, hello_string.c_str(), send_data.dataSize);

	socket.sendData(clientSocket, send_data);

	delete send_data.data;

	SocketBuf data = socket.receiveData(clientSocket, 12);

	for(unsigned int i = 0; i < data.dataSize; ++i)
	{
		printf("Byte %d: %c\n", i, data.data[i]);
	}

	delete data.data;

	std::string pass_required_string = "331 Password required to access user account janne.\r\n";

	send_data.dataSize = strlen(pass_required_string.c_str());
	send_data.data = new char[send_data.dataSize];

	memcpy(send_data.data, pass_required_string.c_str(), send_data.dataSize);

	socket.sendData(clientSocket, send_data);

	data = socket.receiveData(clientSocket, 12);

		for(unsigned int i = 0; i < data.dataSize; ++i)
		{
			printf("Byte %d: %c\n", i, data.data[i]);
		}

		delete data.data;

	delete send_data.data;

	std::string logged_in_string = "230 Logged in, proceed.\r\n";

	send_data.dataSize = strlen(logged_in_string.c_str());
	send_data.data = new char[send_data.dataSize];

	memcpy(send_data.data, logged_in_string.c_str(), send_data.dataSize);

	socket.sendData(clientSocket, send_data);

	delete send_data.data;

	while(1) {}

	return 0;
}
