#include "serial/serial.h"

#include "Libs.h"
#include "Utilities.h"
#include "Packet.h"

#define COM_PORT "COM3"
#define BAUD_RATE 9600

int main()
{
    serial::Serial serial_port(COM_PORT, BAUD_RATE, serial::Timeout::simpleTimeout(100));

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

    while (true)
    {
        SearchTag search_tag(16);
        const auto data = search_tag.GetData();
        auto written_bytes = serial_port.write(data);
        auto response_string = serial_port.read(0xFF);
        auto response_bytes = Utilities::HexStringToBytes(response_string);
        search_tag.ParseResponse(response_bytes);

        if (search_tag.GetResult() == false)
            continue;

        //std::cout << "response_string = " << response_string << "\n";
        std::cout << "card_id = " << Utilities::VectorToString(search_tag.GetId()) << "\n";
    }

    return 0;
}