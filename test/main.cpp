/*
 * main.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: janne
 */

#include "../inc/SocketAPI.h"
#include <string>
#include <thread>

int main(void) {

	SocketAPI socket;

	int serverSocket = socket.getServerSocketFileDescriptor("3370");

	int clientSocket = socket.waitForConnection(serverSocket);

	std::string ok_string = "200 OK.\r\n";

	SocketBuf sendData;
	sendData.dataSize = strlen(ok_string.c_str());
	sendData.data = new char[sendData.dataSize];

	memcpy(sendData.data, ok_string.c_str(), sendData.dataSize);

	socket.sendData(clientSocket, sendData);

	delete sendData.data;

	std::this_thread::sleep_for(std::chrono::milliseconds(40));

	SocketBuf receivedData;
	std::string buf;
	std::string command;
	std::string subCommand;
	while(1){
		receivedData = socket.receiveData(clientSocket, 1);
		if(*receivedData.data != '\r' &&
		   *receivedData.data != '\n' &&
		   *receivedData.data != '\0')
		{
			buf += std::string(receivedData.data);
			delete receivedData.data;
		}
		else
		{
			delete receivedData.data;

			if(buf != "")
			{
				printf("Command received: %s\n", buf.c_str());
				size_t pos = buf.find(' ');
				command = buf.substr(0, pos);
				subCommand = buf.substr(pos + 1, buf.length());
				printf("command: %s, subCommand: %s\n", command.c_str(), subCommand.c_str());

				if("USER" == command)
				{
					printf("Sending 230 ok\n");
					std::string send_string = "230 OK, go ahead\r\n";
					sendData.dataSize = strlen(send_string.c_str());
					sendData.data = new char[sendData.dataSize];

					memcpy(sendData.data, send_string.c_str(), sendData.dataSize);

					socket.sendData(clientSocket, sendData);
					std::this_thread::sleep_for(std::chrono::milliseconds(40));

					delete sendData.data;
				}
				else if("SYST" == command)
				{
					printf("Sending 217 MACOS\n");
					std::string send_string = "217 UNIX\r\n";
					sendData.dataSize = strlen(send_string.c_str());
					sendData.data = new char[sendData.dataSize];

					memcpy(sendData.data, send_string.c_str(), sendData.dataSize);

					socket.sendData(clientSocket, sendData);
					std::this_thread::sleep_for(std::chrono::milliseconds(40));

					delete sendData.data;
				}
				else if("QUIT" == command)
				{
					printf("Sending 221 QUIT response\n");
					std::string send_string = "221 Bye Bye\r\n";
					sendData.dataSize = strlen(send_string.c_str());
					sendData.data = new char[sendData.dataSize];

					memcpy(sendData.data, send_string.c_str(), sendData.dataSize);

					socket.sendData(clientSocket, sendData);
					std::this_thread::sleep_for(std::chrono::milliseconds(40));

					delete sendData.data;

					break;
				}
				else
				{
					printf("Sending 500 not implemented response\n");
					std::string send_string = "500 UNKNOWN COMMAND\r\n";
					sendData.dataSize = strlen(send_string.c_str());
					sendData.data = new char[sendData.dataSize];

					memcpy(sendData.data, send_string.c_str(), sendData.dataSize);

					socket.sendData(clientSocket, sendData);
					std::this_thread::sleep_for(std::chrono::milliseconds(40));

					delete sendData.data;
				}
			}

			buf = "";
		}
	}

	socket.disconnect(clientSocket);
	socket.disconnect(serverSocket);

	return 0;
}
