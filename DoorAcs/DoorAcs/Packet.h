#pragma once

#include "Libs.h"

#include "APDUCommand.h"


// -------- Packet --------
class Packet
{
    PacketType packet_type_;
    string command_number_;
    PacketResponseErrorType packet_response_error_type_;

public:
    Packet(PacketType packet_type, const string& command_number);

    PacketType GetPacketType() const;
    string GetCommandNumber() const;
    PacketResponseErrorType GetResponseError() const;

    virtual string GetData() const = 0;
    virtual void ParseResponse(const vector<BYTE>& response);   // Parses the first byte - error byte
};


// -------- SearchTagPacket --------
class SearchTagPacket : public Packet
{
    BYTE max_id_bytes_;

    bool result_;
    TagType tag_type_;
    BYTE id_bit_count_;
    BYTE id_byte_count_;
    string id_;

public:
    SearchTagPacket(BYTE max_id_bytes);

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;

    BYTE GetMaxIdBytes() const;

    bool GetResult() const;
    TagType GetTagType() const;
    BYTE GetIdBitCount() const;
    BYTE GetIdByteCount() const;
    const string& GetId() const;
};


// -------- SetTagTypesPacket --------
class SetTagTypesPacket : public Packet
{
    TagType lf_;
    TagType hf_;

public:
    SetTagTypesPacket(TagType lf, TagType hf);

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;

    TagType GetLF() const;
    TagType GetHF() const;
};


// -------- CheckPresencePacket --------
class CheckPresencePacket : public Packet
{
    bool result_;

public:
    CheckPresencePacket();

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;

    bool GetResult() const;
};


// -------- ISO14443_4_TDX_Packet --------
class ISO14443_4_TDX_Packet : public Packet
{
    BYTE payload_size_;
    string payload_;
    BYTE maximum_response_size_;

    bool result_;
    BYTE actual_response_size_;
    string response_;

public:
    ISO14443_4_TDX_Packet(BYTE payload_size, const string& payload, BYTE maximum_response_size);
    ISO14443_4_TDX_Packet(const APDUCommand& apdu_command, BYTE maximum_response_size);

    string GetData() const override;
    void ParseResponse(const vector<BYTE>& response) override;

    BYTE GetPayloadSize() const;
    const string& GetPayload() const;
    BYTE GetMaximumResponseSize() const;

    bool GetResult() const;
    BYTE GetActualResponseSize() const;
    const string& GetResponse() const;
};