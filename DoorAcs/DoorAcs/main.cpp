#include "serial/serial.h"
#include <SQLiteCpp/Database.h>

#include "Libs.h"
#include "Utilities.h"
#include "Packet.h"

#include <chrono>

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
    // Tag IDs which are used to grant access to access control system.
    set<string> access_granted_ids = {
        "69B90152",     // NFC tag
        "A001"          // HCE Service of an Android device
    };

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
    string data_string, response_string;
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

    const string cla = "00";
    const string select_ins = "A4";
    const string apdu_p1 = "04";
    const string apdu_p2 = "00";
    const string aid_length = "08"; // 8 bytes
    const string aid = "FF00000000000001"; // AID 8 bytes

    string select_apdu_payload;
    select_apdu_payload += cla;
    select_apdu_payload += select_ins;
    select_apdu_payload += apdu_p1;
    select_apdu_payload += apdu_p2;
    select_apdu_payload += aid_length;
    select_apdu_payload += aid;

    const BYTE select_apdu_payload_size = 13;

    while (true)
    {
        try
        {
            std::cout << "Searching...\n";

            // Send SearchTag
            SearchTag search_tag(16);
            data_string = search_tag.GetData();
            written_bytes = serial_port.write(data_string);
            response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            response_bytes = Utilities::HexStringToBytes(response_string);
            search_tag.ParseResponse(response_bytes);

            // No tag / card found
            if (search_tag.GetResult() == false)
            {
                continue;
            }

            auto tag_id = search_tag.GetIdString();

            std::cout << "response_string = " << response_string << "\n";
            std::cout << "Tag ID = " << tag_id << "\n";

            // Send CheckPresence
            //CheckPresence check_presence; cccheck delete ???
            //data_string = check_presence.GetData();
            //written_bytes = serial_port.write(data_string);
            //response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            //response_bytes = Utilities::HexStringToBytes(response_string);
            //check_presence.ParseResponse(response_bytes);
            //std::cout << "check_presence_response = " << response_string << "\n";
            //std::cout << "presence = " << check_presence.GetResult() << '\n';

            // Send Select APDU
            ISO14443_4_TDX select_apdu(select_apdu_payload_size, select_apdu_payload, MAX_RESPONSE_NUMBER_OF_BYTES);
            data_string = select_apdu.GetData();
            written_bytes = serial_port.write(data_string);
            response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            response_bytes = Utilities::HexStringToBytes(response_string);
            select_apdu.ParseResponse(response_bytes);
            std::cout << "ISO14443_4_TDX_response = " << response_string << "\n";

            // Check if phone responded
            if (select_apdu.GetResult())
            {
                const auto get_apdu_header = "00C0000000";
                string get_card_data = "1203";
                get_card_data += "05";
                get_card_data += get_apdu_header;
                get_card_data += "FF";
                get_card_data += '\r';

                serial_port.write(get_card_data);
                std::cout << "get_card = " << get_card_data << "\n";
                response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
                response_bytes = Utilities::HexStringToBytes(response_string);
                std::cout << "get_card_response = " << response_string << "\n";
            }


        }
        catch (const std::exception& e)
        {
            std::cout << "\n--- EXCEPTION!! ---\n";
            std::cout << "data_string = " << data_string << '\n';
            std::cout << "written_bytes = " << written_bytes << '\n';
            std::cout << "response = " << response_string << '\n';
            std::cout << e.what();
            std::cout << '\n';
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