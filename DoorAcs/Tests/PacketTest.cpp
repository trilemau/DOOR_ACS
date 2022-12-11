#include "catch.hpp"

#include "Packet.h"
#include "Utilities.h"

TEST_CASE("SearchTagPacket")
{
    SECTION("GetData")
    {
        SECTION("MaxIdBytes = 1")
        {
            SearchTagPacket packet(1);

            CHECK(packet.GetPacketType() == PacketType::SEARCH_TAG);
            CHECK(packet.GetCommandNumber() == "0500");
            CHECK(packet.GetData() == "050001\r");
            CHECK(packet.GetMaxIdBytes() == 1);
        }

        SECTION("MaxIdBytes = 16")
        {
            SearchTagPacket packet(16);

            CHECK(packet.GetPacketType() == PacketType::SEARCH_TAG);
            CHECK(packet.GetCommandNumber() == "0500");
            CHECK(packet.GetData() == "050010\r");
            CHECK(packet.GetMaxIdBytes() == 16);
        }

        SECTION("MaxIdBytes = 255")
        {
            SearchTagPacket packet(255);

            CHECK(packet.GetPacketType() == PacketType::SEARCH_TAG);
            CHECK(packet.GetCommandNumber() == "0500");
            CHECK(packet.GetData() == "0500FF\r");
            CHECK(packet.GetMaxIdBytes() == 255);
        }
    }

    SECTION("ParseResponse")
    {
        SearchTagPacket packet(1);

        SECTION("No search tag")
        {
            REQUIRE_NOTHROW(packet.ParseResponse({ 0x00, 0x00 }));

            CHECK(packet.GetResponseError() == PacketResponseErrorType::ERR_NONE);
            CHECK(packet.GetResult() == false);
        }

        SECTION("Invalid responses")
        {
            CHECK_THROWS_AS(packet.ParseResponse({ 0x00 }), runtime_error);
            CHECK_THROWS_AS(packet.ParseResponse({ 0x00, 0x00, 0x00 }), runtime_error);
        }

        SECTION("Tag found")
        {
            REQUIRE_NOTHROW(packet.ParseResponse({ 0x00, 0x01, 0x80, 0x38, 0x07, 0x04, 0x1, 0xF3, 0x8D, 0x29, 0xA5, 0xB, 0x80 }));
            
            CHECK(packet.GetResponseError() == PacketResponseErrorType::ERR_NONE);
            CHECK(packet.GetResult() == true);
            CHECK(packet.GetTagType() == TagType::HFTAG_MIFARE);
            CHECK(packet.GetIdBitCount() == 56);
            CHECK(packet.GetIdByteCount() == 7);

            std::vector<BYTE> expected_id = { 0x04, 0x1, 0xF3, 0x8D, 0x29, 0xA5, 0xB, 0x80 };
            CHECK(Utilities::HexStringToBytes(packet.GetId()) == expected_id);
        }
    }
}

TEST_CASE("SetTagTypesPacket")
{
    SECTION("GetData")
    {
        SECTION("No tag types")
        {
            SetTagTypesPacket packet(TagType::NOTAG, TagType::NOTAG);

            CHECK(packet.GetPacketType() == PacketType::SET_TAG_TYPES);
            CHECK(packet.GetCommandNumber() == "0502");
            CHECK(packet.GetData() == "05020000000000000000\r");
        }

        SECTION("One tag")
        {
            SetTagTypesPacket packet(TagType::NOTAG, TAGMASK(TagType::HFTAG_MIFARE));

            CHECK(packet.GetPacketType() == PacketType::SET_TAG_TYPES);
            CHECK(packet.GetCommandNumber() == "0502");
            CHECK(packet.GetData() == "05020000000001000000\r");
        }

        SECTION("Two tags")
        {
            SetTagTypesPacket packet(TagType::NOTAG, static_cast<TagType>(static_cast<int>(TAGMASK(TagType::HFTAG_MIFARE)) | static_cast<int>(TAGMASK(TagType::HFTAG_ISO14443B))));

            CHECK(packet.GetPacketType() == PacketType::SET_TAG_TYPES);
            CHECK(packet.GetCommandNumber() == "0502");
            CHECK(packet.GetData() == "05020000000003000000\r");
        }

        SECTION("All tag types")
        {
            SetTagTypesPacket packet(TagType::ALL_TAGS, TagType::ALL_TAGS);

            CHECK(packet.GetPacketType() == PacketType::SET_TAG_TYPES);
            CHECK(packet.GetCommandNumber() == "0502");
            CHECK(packet.GetData() == "0502FFFFFFFFFFFFFFFF\r");
        }
    }
}

TEST_CASE("ISO14443_4_TDX_Packet")
{
    SECTION("All args constructor")
    {
        SECTION("GetData")
        {
            SECTION("Empty payload")
            {
                const auto payload_size = 0;
                const string payload = "";

                ISO14443_4_TDX_Packet packet(payload_size, payload, MAX_RESPONSE_NUMBER_OF_BYTES);

                CHECK(packet.GetData() == "120300FF\r");
            }

            SECTION("8 bytes payload")
            {
                const auto payload_size = 8;
                const string payload = "0123456789ABCDEF";

                ISO14443_4_TDX_Packet packet(payload_size, payload, MAX_RESPONSE_NUMBER_OF_BYTES);

                CHECK(packet.GetData() == "1203080123456789ABCDEFFF\r");
            }
        }

        SECTION("ParseResponse")
        {
            const auto payload_size = 8;
            const string payload = "0123456789ABCDEF";

            ISO14443_4_TDX_Packet packet(payload_size, payload, MAX_RESPONSE_NUMBER_OF_BYTES);

            SECTION("Empty response")
            {
                CHECK_THROWS_AS(packet.ParseResponse({}), runtime_error);
            }

            SECTION("Simple Protocol Error")
            {
                vector<BYTE> response{ 0x01 };

                CHECK_THROWS_AS(packet.ParseResponse(response), runtime_error);
                CHECK(packet.GetResponseError() == PacketResponseErrorType::ERR_UNKNOWN_FUNCTION);
            }

            SECTION("Successful response - 0 bytes")
            {
                vector<BYTE> response{ 0x00, 0x01, 0x00 };

                REQUIRE_NOTHROW(packet.ParseResponse(response));

                CHECK(packet.GetResponseError() == PacketResponseErrorType::ERR_NONE);
                CHECK(packet.GetResult() == true);
                CHECK(packet.GetActualResponseSize() == 0);
                CHECK(packet.GetResponse().empty());
            }

            SECTION("Successful response - 8 bytes")
            {
                vector<BYTE> response{ 0x00, 0x01, 0x08, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };

                REQUIRE_NOTHROW(packet.ParseResponse(response));

                CHECK(packet.GetResponseError() == PacketResponseErrorType::ERR_NONE);
                CHECK(packet.GetResult() == true);
                CHECK(packet.GetActualResponseSize() == 8);
                CHECK(packet.GetResponse() == "0123456789ABCDEF");
            }
        }
    }

    SECTION("APDU constructor")
    {
        SECTION("SelectFileAPDUCommand")
        {
            const string aid = "FF00000000000001"; // AID 8 bytes

            SelectFileAPDUCommand apdu(aid);

            ISO14443_4_TDX_Packet packet(apdu, MAX_RESPONSE_NUMBER_OF_BYTES);

            SECTION("GetData")
            {
                CHECK(packet.GetData() == "12030E00A4040008FF00000000000001FFFF\r");
            }
        }

        SECTION("GetResponseAPDUCommand")
        {
            GetResponseAPDUCommand apdu;

            ISO14443_4_TDX_Packet packet(apdu, MAX_RESPONSE_NUMBER_OF_BYTES);

            SECTION("GetData")
            {
                CHECK(packet.GetData() == "12030600C0000000FFFF\r");
            }
        }
    }
}
