#include "Utilities.h"

int Utilities::ASCIICharToHex(char ascii_char)
{
    switch (ascii_char)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
        return 0xA;
    case 'B':
        return 0xB;
    case 'C':
        return 0xC;
    case 'D':
        return 0xD;
    case 'E':
        return 0xE;
    case 'F':
        return 0xF;
    default:
        throw std::invalid_argument("[ASCIIChartToHex] invalid ASCII char");
    }
}

vector<BYTE> Utilities::HexStringToBytes(const string& byte_string)
{
    vector<BYTE> output;
    output.resize(byte_string.size() / 2);

    for (size_t i = 0; i < output.size(); i++)
        output[i] = (ASCIICharToHex(byte_string[2 * i]) << 4) | (ASCIICharToHex(byte_string[2 * i + 1]));

    return output;
}

string Utilities::VectorToString(const vector<BYTE>& vector)
{
    stringstream stream;

    stream << '[' << std::setfill('0') << std::hex;

    for (size_t i = 0; i < vector.size(); i++)
    {
        stream << std::setw(2) << static_cast<int>(vector[i]);

        if (i < vector.size() - 1)
            stream << std::setw(1) << ' ';
    }

    stream << ']';

    return stream.str();
}