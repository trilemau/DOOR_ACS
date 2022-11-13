#include "APDUResponse.h"

#include "Utilities.h"

// -------- APDUResponse --------
APDUResponse::APDUResponse(APDUCommandType apdu_command_type, const string& response)
	: apdu_command_type_(apdu_command_type)
{
	// SW1 and SW2 is mandatory
	if (response.length() < 4)
	{
		std::cout << response << '\n';
		throw std::runtime_error("APDUResponse invalid response");
	}

	status_.reserve(4);

	// SW1
	status_ += response[response.length() - 4];
	status_ += response[response.length() - 3];

	// SW2
	status_ += response[response.length() - 2];
	status_ += response[response.length() - 1];

	// Data
	data_ = response.substr(0, response.size() - 4);
}

APDUCommandType APDUResponse::GetAPDUCommandType() const
{
	return apdu_command_type_;
}

const string& APDUResponse::GetData() const
{
	return data_;
}

const string& APDUResponse::GetStatus() const
{
	return status_;
}


// -------- SelectFileAPDUResponse --------
SelectFileAPDUResponse::SelectFileAPDUResponse(const string& response)
	: APDUResponse(APDUCommandType::SELECT_FILE, response)
{

}


// -------- GetResponseAPDUResponse --------
GetResponseAPDUResponse::GetResponseAPDUResponse(const string& response)
	: APDUResponse(APDUCommandType::GET_RESPONSE, response)
{
	// data from response consists of only ID from HCE Service
	id_ = GetData();
}

string GetResponseAPDUResponse::GetId() const
{
	return id_;
}