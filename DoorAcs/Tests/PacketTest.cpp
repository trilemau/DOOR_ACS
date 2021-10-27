#include "catch.hpp"

#include "Packet.h"

TEST_CASE("SearchTag")
{
    SECTION("GetData")
    {
        SECTION("MaxIdBytes = 1")
        {
            SearchTag packet(1);

            const auto result = packet.GetData();
            CHECK(result == "050001\r");
        }

        SECTION("MaxIdBytes = 16")
        {
            SearchTag packet(16);

            const auto result = packet.GetData();
            CHECK(result == "050010\r");
        }

        SECTION("MaxIdBytes = 255")
        {
            SearchTag packet(255);

            const auto result = packet.GetData();
            CHECK(result == "0500ff\r");
        }
    }

    SECTION("ParseResponse")
    {
        SearchTag packet(1);

        SECTION("No search tag")
        {
            REQUIRE_NOTHROW(packet.ParseResponse({ 0x00, 0x00 }));
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
            CHECK(packet.GetResult() == true);
            CHECK(packet.GetTagType() == TagType::HFTAG_MIFARE);
            CHECK(packet.GetIdBitCount() == 56);
            CHECK(packet.GetIdByteCount() == 7);

            std::vector<BYTE> expected_id = { 0x04, 0x1, 0xF3, 0x8D, 0x29, 0xA5, 0xB, 0x80 };
            CHECK(packet.GetId() == expected_id);
        }
    }
}

TEST_CASE("SetTagTypes")
{
    SECTION("GetData")
    {
        SECTION("No tag types")
        {
            SetTagTypes packet(TagType::NOTAG, TagType::NOTAG);

            const auto result = packet.GetData();
            CHECK(result == "05020000000000000000\r");
        }

        SECTION("One tag")
        {
            SetTagTypes packet(TagType::NOTAG, TAGMASK(TagType::HFTAG_MIFARE));

            const auto result = packet.GetData();
            CHECK(result == "05020000000001000000\r");
        }

        SECTION("Two tags")
        {
            SetTagTypes packet(TagType::NOTAG, static_cast<TagType>(static_cast<int>(TAGMASK(TagType::HFTAG_MIFARE)) | static_cast<int>(TAGMASK(TagType::HFTAG_ISO14443B))));

            const auto result = packet.GetData();
            CHECK(result == "05020000000003000000\r");
        }

        SECTION("All tag types")
        {
            SetTagTypes packet(TagType::ALL_TAGS, TagType::ALL_TAGS);

            const auto result = packet.GetData();
            CHECK(result == "0502ffffffffffffffff\r");
        }
    }
}