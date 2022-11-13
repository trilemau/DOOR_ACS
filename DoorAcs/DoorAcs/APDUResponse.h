#pragma once

#include "Libs.h"

// -------- APDUResponse --------
class APDUResponse
{
	APDUCommandType apdu_command_type_;	// type of APDU command which the response returned from
	string data_;
	string status_; // sw1 and sw2

public:
	APDUResponse(APDUCommandType apdu_command_type, const string& response);

	APDUCommandType GetAPDUCommandType() const;
	const string& GetData() const;
	const string& GetStatus() const;
};


// -------- SelectFileAPDUResponse --------
class SelectFileAPDUResponse : public APDUResponse
{
public:
	SelectFileAPDUResponse(const string& response);
};


// -------- GetResponseAPDUResponse --------
class GetResponseAPDUResponse : public APDUResponse
{
	string id_; // ID from HCE Service

public:
	GetResponseAPDUResponse(const string& response);

	string GetId() const;
};