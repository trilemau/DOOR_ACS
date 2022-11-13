#include "serial/serial.h"
#include <SQLiteCpp/Database.h>

#include "Libs.h"
#include "Utilities.h"
#include "Packet.h"
#include "APDUResponse.h"

#include <chrono>


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

    auto serial_port_timeout = serial::Timeout::simpleTimeout(700);
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
    bool was_hce_used = false;
    int written_bytes;
    string data_string, response_string;
    vector<BYTE> response_bytes;

    // Flush input / output
    serial_port.flush();

    try
    {
        // Send SetTagTypesPacket
        SetTagTypesPacket set_tag_types(TagType::NOTAG, TagType::ALL_TAGS);
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

    while (true)
    {
        try
        {
            std::cout << "Searching...\n";

            was_hce_used = false;

            // Send SearchTagPacket
            SearchTagPacket search_tag(16);
            data_string = search_tag.GetData();
            written_bytes = serial_port.write(data_string);
            response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            response_bytes = Utilities::HexStringToBytes(response_string);
            search_tag.ParseResponse(response_bytes);
            //std::cout << "response_string = " << response_string << "\n"; // Uncomment for debugging

            // No tag / card found
            if (search_tag.GetResult() == false)
            {
                continue;
            }

            auto id = search_tag.GetId();

            // Send SELECT FILE APDU command
            SelectFileAPDUCommand select_file_apdu_command(HCE_SERVICE_AID);
            ISO14443_4_TDX_Packet select_file_tdx_packet(select_file_apdu_command, MAX_RESPONSE_NUMBER_OF_BYTES);
            data_string = select_file_tdx_packet.GetData();
            written_bytes = serial_port.write(data_string);
            response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
            response_bytes = Utilities::HexStringToBytes(response_string);
            select_file_tdx_packet.ParseResponse(response_bytes);
            //std::cout << "select_file_response = " << response_string << "\n"; // Uncomment for debugging

            // Parse SELECT FILE APDU response only when command was sent successfully
            if (select_file_tdx_packet.GetResult())
            {
                // Parse SELECT FILE APDU response
                SelectFileAPDUResponse select_file_apdu_response(select_file_tdx_packet.GetResponse());

                // Check if phone responded with STATUS OK
                if (select_file_apdu_response.GetStatus() == APDU_STATUS_OK)
                {
                    ISO14443_4_TDX_Packet get_response_tdx_packet(GetResponseAPDUCommand(), MAX_RESPONSE_NUMBER_OF_BYTES);
                    data_string = get_response_tdx_packet.GetData();
                    written_bytes = serial_port.write(data_string);
                    response_string = serial_port.read(MAX_RESPONSE_NUMBER_OF_BYTES);
                    response_bytes = Utilities::HexStringToBytes(response_string);
                    get_response_tdx_packet.ParseResponse(response_bytes);
                    std::cout << "get_response_response = " << response_string << "\n"; // Uncomment for debugging

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

            // TODO add check for access granted
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