/*
 * SocketAPI.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: janne
 */

#include "../inc/SocketAPI.h"

int SocketAPI::waitForConnection(const std::string& address, const std::string& portNo) {
	return -1;
}

int SocketAPI::connect(const std::string& address, const std::string& portNo) {
	return -1;
}

SocketBuf SocketAPI::receive_data(int fileDescriptor, int noOfBytes) {
	SocketBuf retVal;

	return retVal;
}

int SocketAPI::sendData(int fileDescriptor, const SocketBuf& socketBuf) {

	return -1;
}
