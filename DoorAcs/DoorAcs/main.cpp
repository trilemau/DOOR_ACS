#include "serial/serial.h"

#include "Libs.h"
#include "Utilities.h"
#include "Packet.h"

#define COM_PORT "COM3"
#define BAUD_RATE 9600

int main()
{
    serial::Serial serial_port(COM_PORT, BAUD_RATE, serial::Timeout::simpleTimeout(1000));

    if (serial_port.isOpen())
    {
        std::cout << "Serial port opened successfully...\n";
    }
    else
    {
        std::cout << "Serial port fail...\n";
        return -1;
    }

    // SetTagTypes
    serial_port.write("05020000000003000000\r");
    auto response_string = serial_port.read(0xFF);

    if (response_string != "00\r")
        std::cout << "SetTagTypes failed";

    string check_presence_data = "1202\r";

    const auto apdu = "00A4040010"; // APDU header
    const auto aid = "FF3F12C1583A69D28C4082412A90AC00"; // AID
    string select_data = "1203";
    select_data += "15"; // 21 in HEX
    select_data += apdu;
    select_data += aid;
    select_data += "15"; // 21 in HEX
    select_data += '\r';


    while (true)
    {
        try
        {
            SearchTag search_tag(16);
            const auto data = search_tag.GetData();
            auto written_bytes = serial_port.write(data);
            auto response_string = serial_port.read(0xFF);
            auto response_bytes = Utilities::HexStringToBytes(response_string);
            search_tag.ParseResponse(response_bytes);

            if (search_tag.GetResult() == false)
                continue;

            std::cout << "response_string = " << response_string << "\n";
            std::cout << "card_id = " << Utilities::VectorToString(search_tag.GetId()) << "\n";

            // Send CheckPresence
            //serial_port.write(check_presence_data);
            //response_string = serial_port.read(0xFF);
            //response_bytes = Utilities::HexStringToBytes(response_string);
            //std::cout << "check_presence_response = " << response_string << "\n";

            // Send Select APDU
            serial_port.write(select_data);
            response_string = serial_port.read(0xFF);
            response_bytes = Utilities::HexStringToBytes(response_string);
            std::cout << "select_apdu_response = " << response_string << "\n";

            std::cout << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << e.what();
        }
    }

    return 0;
}