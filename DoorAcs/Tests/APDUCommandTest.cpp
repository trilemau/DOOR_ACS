#include "catch.hpp"

#include "APDUCommand.h"
#include "Utilities.h"

TEST_CASE("SelectFileAPDUCommand")
{
    const string aid = "FF00000000000001"; // AID 8 bytes

    SelectFileAPDUCommand apdu_command(aid);

    SECTION("Attributes")
    {
        CHECK(apdu_command.GetAPDUCommandType() == APDUCommandType::SELECT_FILE);
        CHECK(apdu_command.GetCla() == 0x00);
        CHECK(apdu_command.GetIns() == 0xA4);
        CHECK(apdu_command.GetParameter1() == 0x04);
        CHECK(apdu_command.GetParameter2() == 0x00);
        CHECK(apdu_command.GetDataLength() == 8);
        CHECK(apdu_command.GetData() == aid);
        CHECK(apdu_command.GetResponseLength() == 0xFF);

        CHECK(apdu_command.GetAid() == aid);
    }

    SECTION("Header")
    {
        CHECK(apdu_command.GetHeader() == "00A4040008");
    }

    SECTION("Payload")
    {
        CHECK(apdu_command.GetPayload() == "00A4040008FF00000000000001FF");
        CHECK(apdu_command.GetPayloadSize() == 14);
    }
}

TEST_CASE("GetResponseAPDUCommand")
{
    GetResponseAPDUCommand apdu_command;

    SECTION("Attributes")
    {
        CHECK(apdu_command.GetAPDUCommandType() == APDUCommandType::GET_RESPONSE);
        CHECK(apdu_command.GetCla() == 0x00);
        CHECK(apdu_command.GetIns() == 0xC0);
        CHECK(apdu_command.GetParameter1() == 0x00);
        CHECK(apdu_command.GetParameter2() == 0x00);
        CHECK(apdu_command.GetDataLength() == 0);
        CHECK(apdu_command.GetData() == "");
        CHECK(apdu_command.GetResponseLength() == 0xFF);
    }

    SECTION("Header")
    {
        CHECK(apdu_command.GetHeader() == "00C0000000");
    }

    SECTION("Payload")
    {
        CHECK(apdu_command.GetPayload() == "00C0000000FF");
        CHECK(apdu_command.GetPayloadSize() == 6);
    } 
}