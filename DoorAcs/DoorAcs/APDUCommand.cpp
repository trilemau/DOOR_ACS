#include "APDUCommand.h"

#include "Utilities.h"


BYTE GetInstructionFromAPDUCommandType(APDUCommandType apdu_command_type)
{
	switch (apdu_command_type)
	{
	case APDUCommandType::SELECT_FILE:
		return INS_SELECT_FILE;
	case APDUCommandType::GET_RESPONSE:
		return INS_GET_RESPONSE;
	default:
		break;
	}

	throw runtime_error("Invalid APDUCommandType = " + static_cast<BYTE>(apdu_command_type));
}


// -------- APDUCommand --------
APDUCommand::APDUCommand(APDUCommandType apdu_command_type, BYTE cla, BYTE parameter1, BYTE parameter2, BYTE response_length)
	: apdu_command_type_(apdu_command_type)
	, cla_(cla)
	, ins_(GetInstructionFromAPDUCommandType(apdu_command_type))
	, parameter1_(parameter1)
	, parameter2_(parameter2)
	, data_length_(0)
	, response_length_(response_length)
{

}

BYTE APDUCommand::GetPayloadSize() const
{
	// HEADER + DATA_LENGTH + 1 byte response length
	return APDU_HEADER_LENGTH + GetDataLength() + 1;
}

string APDUCommand::GetHeader() const
{
	// Construct APDU command header
	stringstream ss;

	ss << std::hex << std::setfill('0');
	ss << std::uppercase;

	ss << std::setw(2) << (cla_ & 0xFF);
	ss << std::setw(2) << (ins_ & 0xFF);
	ss << std::setw(2) << (parameter1_ & 0xFF);
	ss << std::setw(2) << (parameter2_ & 0xFF);
	ss << std::setw(2) << (data_length_ & 0xFF);

	return ss.str();
}

void APDUCommand::SetData(const string& data)
{
	data_length_ = data.size() / 2;
	data_ = data;
}

APDUCommandType APDUCommand::GetAPDUCommandType() const
{
	return apdu_command_type_;
}

BYTE APDUCommand::GetCla() const
{
	return cla_;
}

BYTE APDUCommand::GetIns() const
{
	return ins_;
}

BYTE APDUCommand::GetParameter1() const
{
	return parameter1_;
}

BYTE APDUCommand::GetParameter2() const
{
	return parameter2_;
}

BYTE APDUCommand::GetDataLength() const
{
	return data_length_;
}

const string& APDUCommand::GetData() const
{
	return data_;
}

BYTE APDUCommand::GetResponseLength() const
{
	return response_length_;
}


// -------- SelectFileAPDUCommand --------
SelectFileAPDUCommand::SelectFileAPDUCommand(const string& aid)
	: APDUCommand(APDUCommandType::SELECT_FILE, DEFAULT_CLA, P1_SELECT_FILE, NO_PARAMETER, MAX_RESPONSE_NUMBER_OF_BYTES)
	, aid_(aid)
{
	SetData(aid);
}

string SelectFileAPDUCommand::GetPayload() const
{
	stringstream ss;
	
	ss << std::hex << std::setfill('0');
	ss << std::uppercase;

	ss << GetHeader();
	ss << GetData();
	ss << std::setw(2) << MAX_RESPONSE_NUMBER_OF_BYTES;

	return ss.str();
}

const string& SelectFileAPDUCommand::GetAid() const
{
	return aid_;
}


// -------- GetResponseAPDUCommand --------
GetResponseAPDUCommand::GetResponseAPDUCommand()
	: APDUCommand(APDUCommandType::GET_RESPONSE, DEFAULT_CLA, NO_PARAMETER, NO_PARAMETER, MAX_RESPONSE_NUMBER_OF_BYTES)
{
	SetData("");
}

string GetResponseAPDUCommand::GetPayload() const
{
	stringstream ss;

	ss << std::hex << std::setfill('0');
	ss << std::uppercase;

	ss << GetHeader();
	ss << std::setw(2) << MAX_RESPONSE_NUMBER_OF_BYTES;

	return ss.str();
}