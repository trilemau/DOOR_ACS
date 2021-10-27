#pragma once

#include "Libs.h"


// -------- enum PacketType --------
enum class PacketType
{
    GET_VERSION_STRING,
    SEARCH_TAG,
    SET_PARAMETERS,
    SET_TAG_TYPES
};


// -------- Packet --------
class Packet
{
    PacketType type_;

public:
    Packet(PacketType type);

    virtual string GetData() const;
    virtual void ParseResponse(const vector<BYTE>& response);
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