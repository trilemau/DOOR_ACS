#pragma once

#include "Libs.h"
#include "serial/serial.h"

class Reader
{
	serial::Serial serial_port_;
	string data_string_;
	int written_bytes_;
	string response_string_;

	void InitialiseSerialPort();
	
public:
	Reader();

	string GetID();
	void FlushSerialPort();

	string GetDataString() const;
	int GetWrittenBytes() const;
	string GetResponseString() const;
};