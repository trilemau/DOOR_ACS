#pragma once

#include "Libs.h"


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
    const string GetIdString() const;
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

    bool GetResult() const;
};


// -------- ISO14443_4_TDX --------
class ISO14443_4_TDX : public Packet
{
    BYTE payload_size_;
    string payload_;
    BYTE maximum_response_size_;
    bool result_;
    BYTE response_size_;
    vector<BYTE> response_;

public:
    ISO14443_4_TDX(BYTE payload_size, const string& payload, BYTE maximum_response_size);

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;

    BYTE GetPayloadSize() const;
    const string& GetPayload() const;
    BYTE GetMaximumResponseSize() const;
    bool GetResult() const;
    BYTE GetResponseSize() const;
    const vector<BYTE>& GetResponse() const;
};