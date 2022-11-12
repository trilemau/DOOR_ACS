#pragma once

#include "Libs.h"


// -------- enum PacketType --------
enum class PacketType
{
    SEARCH_TAG,
    SET_TAG_TYPES,
    CHECK_PRESENCE,
    ISO14443_4_TDX
};

// -------- enum PacketType --------
enum class PacketResponseErrorType
{
    ERR_NONE = 0,
    ERR_UNKNOWN_FUNCTION = 1,
    ERR_MISSING_PARAMETER = 2,
    ERR_UNUSED_PARAMETERS = 3,
    ERR_INVALID_FUNCTION = 4,
    ERR_PARSER = 5
};



// -------- Packet --------
class Packet
{
    PacketType packet_type_;
    string command_number_;
    PacketResponseErrorType packet_response_error_type_;

public:
    Packet(PacketType packet_type, const string& command_number);

    virtual PacketType GetPacketType() const;
    virtual string GetCommandNumber() const;

    virtual string GetData() const = 0;
    virtual void ParseResponse(const vector<BYTE>& response);   // Parses the first byte - error byte
};


// -------- SearchTag --------
class SearchTag : public Packet
{
    BYTE max_id_bytes_;

    bool result_;
    TagType tag_type_;
    BYTE id_bit_count_;
    BYTE id_byte_count_;
    vector<BYTE> id_;

public:
    SearchTag(BYTE max_id_bytes);

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;

    BYTE GetMaxIdBytes() const;
    bool GetResult() const;
    TagType GetTagType() const;
    BYTE GetIdBitCount() const;
    BYTE GetIdByteCount() const;
    const vector<BYTE>& GetId() const;
};


// -------- SetTagTypes --------
class SetTagTypes : public Packet
{
    TagType lf_;
    TagType hf_;

public:
    SetTagTypes(TagType lf, TagType hf);

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;

    TagType GetLF() const;
    TagType GetHF() const;
};


// -------- CheckPresence --------
class CheckPresence : public Packet
{
    bool result_;

public:
    CheckPresence();

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;
};


// -------- ISO14443_4_TDX --------