/*
 * SocketAPI.h
 *
 *  Created on: Aug 17, 2016
 *      Author: janne
 */

#ifndef INC_SOCKETAPI_H_
#define INC_SOCKETAPI_H_

#include <string>

struct SocketBuf
{
	int dataSize;
	char* data;
};

class SocketAPI
{
public:
	int connect(const std::string& address, const std::string& portNo);

	int waitForConnection(const std::string& address, const std::string& portNo);

	SocketBuf receive_data(int fileDescriptor, int noOfBytes);

	int sendData(int fileDescriptor, const SocketBuf& socketBuf);

protected:

private:
	int socketFd;
};


#endif /* INC_SOCKETAPI_H_ */
