#include "Packet.h"
#include "Utilities.h"


// -------- Packet --------
Packet::Packet(PacketType type)
    : type_(type)
{

}

string Packet::GetData() const
{
    return "";
}

void Packet::ParseResponse(const vector<BYTE>& response)
{

}


// -------- SearchTag --------
SearchTag::SearchTag(BYTE max_id_bytes)
    : Packet(PacketType::SEARCH_TAG)
    , max_id_bytes_(max_id_bytes)
    , result_(0)
    , tag_type_(TagType::NOTAG)
    , id_bit_count_(0)
    , id_byte_count_(0)
{

}

string SearchTag::GetData() const
{
    stringstream ss;
    ss << "0500" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(max_id_bytes_) << '\r';
    return ss.str();
}

void SearchTag::ParseResponse(const vector<BYTE>& response)
{
    // Response has to be atleast 2 bytes - NO TAG FOUND [0x0, 0x0]
    if (response.size() < 2 || response[0] != 0 || response[1] == 0 && response.size() != 2)
    {
        std::cout << Utilities::VectorToString(response);
        throw std::runtime_error("SearchTag invalid response");
    }

    result_ = response[1];

    // NO TAG FOUND
    if (result_ == false)
        return;

    if (response.size() < 5)
    {
        std::cout << Utilities::VectorToString(response);
        throw std::runtime_error("SearchTag invalid response - tag found");
    }

    tag_type_ = static_cast<TagType>(response[2]);
    id_bit_count_ = response[3];
    id_byte_count_ = response[4];

    if (response.size() < 5 + id_byte_count_)
    {
        std::cout << Utilities::VectorToString(response);
        throw std::runtime_error("SearchTag invalid response - tag length");
    }

    id_.assign(response.begin() + 5, response.end());
}

BYTE SearchTag::GetMaxIdBytes() const
{
    return max_id_bytes_;
}

bool SearchTag::GetResult() const
{
    return result_;
}

TagType SearchTag::GetTagType() const
{
    return tag_type_;
}

BYTE SearchTag::GetIdBitCount() const
{
    return id_bit_count_;
}

BYTE SearchTag::GetIdByteCount() const
{
    return id_byte_count_;
}

const vector<BYTE>& SearchTag::GetId() const
{
    return id_;
}


// -------- SetTagTypes --------
SetTagTypes::SetTagTypes(TagType lf, TagType hf)
    : Packet(PacketType::SET_TAG_TYPES)
    , lf_(lf)
    , hf_(hf)
{

}

string SetTagTypes::GetData() const
{
    stringstream ss;
    ss << "0502" << std::hex << std::setfill('0');

    // LF
    ss << std::setw(2) << (static_cast<int>(lf_) & 0xFF);
    ss << std::setw(2) << ((static_cast<int>(lf_) & 0xFF00) >> 8);
    ss << std::setw(2) << ((static_cast<int>(lf_) & 0xFF0000) >> 16);
    ss << std::setw(2) << ((static_cast<int>(lf_) & 0xFF000000) >> 24);

    // HF
    ss << std::setw(2) << (static_cast<int>(hf_) & 0xFF);
    ss << std::setw(2) << ((static_cast<int>(hf_) & 0xFF00) >> 8);
    ss << std::setw(2) << ((static_cast<int>(hf_) & 0xFF0000) >> 16);
    ss << std::setw(2) << ((static_cast<int>(hf_) & 0xFF000000) >> 24);

    ss << '\r';

    return ss.str();
}

void SetTagTypes::ParseResponse(const vector<BYTE>& response)
{
    // TODO
}

TagType SetTagTypes::GetLF() const
{
    return lf_;
}

TagType SetTagTypes::GetHF() const
{
    return hf_;
}