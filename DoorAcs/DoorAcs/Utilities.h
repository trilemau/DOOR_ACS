#pragma once

#include "Libs.h"

class Utilities
{
public:
    static int ASCIICharToHex(char ascii_char);
    static vector<BYTE> HexStringToBytes(const string& byte_string);
    static string VectorToString(const vector<BYTE>& vector);
};