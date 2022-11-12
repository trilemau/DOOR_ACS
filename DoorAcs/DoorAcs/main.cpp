#include "serial/serial.h"
#include <SQLiteCpp/Database.h>

#include "Libs.h"
#include "Utilities.h"
#include "Packet.h"

#define COM_PORT "COM3"
#define BAUD_RATE 9600
#define MAX_RESPONSE_NUMBER_OF_BYTES 0xFF

#define NO_HF_TAG_TYPES 0
#define ALL_LF_TAG_TYPES MAX_RESPONSE_NUMBER_OF_BYTESFFFFFF

#define NO_LF_TAG_TYPES 0
#define ALL_HF_TAG_TYPES MAX_RESPONSE_NUMBER_OF_BYTESFFFFFF

void sqlTest()
{

}

int cardReader()
{
    auto serial_port_timeout = serial::Timeout::simpleTimeout(500);
    serial_port_timeout.inter_byte_timeout = 0;

    serial::Serial serial_port(COM_PORT, BAUD_RATE, serial_port_timeout);

    if (serial_port.isOpen())
    {
        std::cout << "Serial port opened successfully...\n";
    }
    else
    {
        std::cout << "Serial port fail...\n";
        return EXIT_FAILURE;
    }

    // variable declarations
    int written_bytes;
    string response_string;
    vector<BYTE> response_bytes;

    // Flush input / output
    serial_port.flush();

    try
    {
        // Send SetTagTypes
        SetTagTypes set_tag_types(TagType::NOTAG, TagType::ALL_TAGS);
        const auto set_tag_types_data = set_tag_types.GetData();
        written_bytes = serial_port.write(set_tag_types_data);
        response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
        response_bytes = Utilities::HexStringToBytes(response_string);
        set_tag_types.ParseResponse(response_bytes);
    }
    catch (const std::exception& e)
    {
        std::cout << "Failed to set tag types\n";
        return EXIT_FAILURE;
    }

    const auto select_apdu = "00A4040010"; // APDU header
    const auto aid = "FF3F12C1583A69D28C4082412A90AC00"; // AID
    string select_data = "1203";
    select_data += "15"; // 21 in HEX
    select_data += select_apdu;
    select_data += aid;
    select_data += "15"; // 21 in HEX
    select_data += '\r';

    while (true)
    {
        try
        {
            std::cout << "Searching...\n";

            // Send SearchTag
            SearchTag search_tag(16);
            const auto search_tag_data = search_tag.GetData();
            written_bytes = serial_port.write(search_tag_data);
            response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            response_bytes = Utilities::HexStringToBytes(response_string);
            search_tag.ParseResponse(response_bytes);

            // No tag / card found
            if (search_tag.GetResult() == false)
                continue;

            std::cout << "response_string = " << response_string << "\n";
            std::cout << "card_id = " << Utilities::VectorToString(search_tag.GetId()) << "\n";

            // Send CheckPresence
            CheckPresence check_presence;
            string check_presence_data = check_presence.GetData();
            serial_port.write(check_presence_data);
            response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            response_bytes = Utilities::HexStringToBytes(response_string);
            std::cout << "check_presence_response = " << response_string << "\n";

            //// Send Select APDU
            //std::cout << "select_apdu = " << select_data << "\n";
            //serial_port.write(select_data);
            //response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            //response_bytes = Utilities::HexStringToBytes(response_string);
            //std::cout << "select_apdu_response = " << response_string << "\n";

            //// Check if phone responded
            //if (response_string == "0001029000\r")
            //{
            //    const auto get_apdu = "00C0000010";
            //    const auto gate_id = "AAAAAAAAAAAAAAAA";
            //    const auto gate_salt = "BBBBBBBBBBBBBBBB";
            //    string get_card_data = "1203";
            //    get_card_data += "15";
            //    get_card_data += get_apdu;
            //    get_card_data += gate_id;
            //    get_card_data += gate_salt;
            //    get_card_data += "15";
            //    get_card_data += '\r';

            //    //serial_port.write(get_card_data);
            //    //std::cout << "get_card = " << get_card_data << "\n";
            //    //response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            //    //response_bytes = Utilities::HexStringToBytes(response_string);
            //    //std::cout << "get_card_response = " << response_string << "\n";
            //}
        }
        catch (const std::exception& e)
        {
            std::cout << "response = " << response_string << '\n';
            std::cout << e.what();
            serial_port.flush();
        }

        std::cout << "\n";
    }

    return EXIT_SUCCESS;
}

int main()
{
    //sqlTest();

    auto result = cardReader();

    return result;
}