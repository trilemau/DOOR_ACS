#include "Reader.hpp"

#include "APDUCommand.h"
#include "APDUResponse.h"
#include "Packet.h"
#include "Utilities.h"

Reader::Reader()
    : serial_port_(COM_PORT, BAUD_RATE, serial::Timeout::simpleTimeout(SERIAL_PORT_TIMEOUT))
    , written_bytes_(0)
{
	InitialiseSerialPort();

    // Send SetTagTypesPacket
    SetTagTypesPacket set_tag_types(TagType::NOTAG, TagType::ALL_TAGS);
    const auto set_tag_types_data = set_tag_types.GetData();
    const auto written_bytes_ = serial_port_.write(set_tag_types_data);
    const auto response_string_ = serial_port_.read(MAX_RESPONSE_NUMBER_OF_BYTES);
    const auto response_bytes = Utilities::HexStringToBytes(response_string_);
    set_tag_types.ParseResponse(response_bytes);
}

string Reader::GetID()
{
    std::cout << "Searching...\n";

    auto was_hce_used = false;

    // Send SearchTagPacket
    SearchTagPacket search_tag(16);
    data_string_ = search_tag.GetData();
    written_bytes_ = serial_port_.write(data_string_);
    response_string_ = serial_port_.read(MAX_RESPONSE_NUMBER_OF_BYTES);
    auto response_bytes = Utilities::HexStringToBytes(response_string_);
    search_tag.ParseResponse(response_bytes);
    //std::cout << "response_string_ = " << response_string_ << "\n"; // Uncomment for debugging

    // No tag / card found
    if (search_tag.GetResult() == false)
    {
        return "";
    }

    auto id = search_tag.GetId();

    // Send SELECT FILE APDU command
    SelectFileAPDUCommand select_file_apdu_command(HCE_SERVICE_AID);
    ISO14443_4_TDX_Packet select_file_tdx_packet(select_file_apdu_command, MAX_RESPONSE_NUMBER_OF_BYTES);
    data_string_ = select_file_tdx_packet.GetData();
    written_bytes_ = serial_port_.write(data_string_);
    response_string_ = serial_port_.read(MAX_RESPONSE_NUMBER_OF_BYTES);
    response_bytes = Utilities::HexStringToBytes(response_string_);
    select_file_tdx_packet.ParseResponse(response_bytes);
    //std::cout << "select_file_response = " << response_string_ << "\n"; // Uncomment for debugging

    // Parse SELECT FILE APDU response only when command was sent successfully
    if (select_file_tdx_packet.GetResult())
    {
        // Parse SELECT FILE APDU response
        SelectFileAPDUResponse select_file_apdu_response(select_file_tdx_packet.GetResponse());

        // Check if phone responded with STATUS OK
        if (select_file_apdu_response.GetStatus() == APDU_STATUS_OK)
        {
            ISO14443_4_TDX_Packet get_response_tdx_packet(GetResponseAPDUCommand(), MAX_RESPONSE_NUMBER_OF_BYTES);
            data_string_ = get_response_tdx_packet.GetData();
            written_bytes_ = serial_port_.write(data_string_);
            response_string_ = serial_port_.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            response_bytes = Utilities::HexStringToBytes(response_string_);
            get_response_tdx_packet.ParseResponse(response_bytes);
            //std::cout << "get_response_response = " << response_string_ << "\n"; // Uncomment for debugging

            // Parse GET RESPONSE APDU response only when command was sent successfully
            if (get_response_tdx_packet.GetResult())
            {
                GetResponseAPDUResponse get_response_apdu_response(get_response_tdx_packet.GetResponse());
                id = get_response_apdu_response.GetId();
                was_hce_used = true;
            }
        }
    }

    if (was_hce_used == true)
    {
        std::cout << "HCE Service ID = " << id << "\n";
    }
    else
    {
        std::cout << "Tag ID = " << id << "\n";
    }

    return id;
}

void Reader::FlushSerialPort()
{
    serial_port_.flush();
}

string Reader::GetDataString() const
{
    return data_string_;
}

int Reader::GetWrittenBytes() const
{
    return written_bytes_;
}

string Reader::GetResponseString() const
{
    return response_string_;
}

void Reader::InitialiseSerialPort()
{
    // Set timeouts
    auto timeout = serial_port_.getTimeout();
    timeout.inter_byte_timeout = 0;
    serial_port_.setTimeout(timeout);

    // Open serial port
    if (serial_port_.isOpen())
    {
        std::cout << "Serial port opened successfully...\n";
    }
    else
    {
        throw std::runtime_error("Serial port failed to open.");
    }

    // Flush input / output of serial port
    FlushSerialPort();
}