#include "Packet.h"

#include "Utilities.h"


// -------- Packet --------
Packet::Packet(PacketType packet_type, const string& command_number)
    : packet_type_(packet_type)
    , command_number_(command_number)
    , packet_response_error_type_(PacketResponseErrorType::ERR_NONE)
{

}

PacketType Packet::GetPacketType() const
{
    return packet_type_;
}

string Packet::GetCommandNumber() const
{
    return command_number_;
}

PacketResponseErrorType Packet::GetResponseError() const
{
    return packet_response_error_type_;
}

void Packet::ParseResponse(const vector<BYTE>& response)
{
    // Response has to be atleast 1 byte long - error byte
    if (response.size() < 1)
    {
        throw std::runtime_error("Packet invalid response");
    }

    packet_response_error_type_ = static_cast<PacketResponseErrorType>(response[0]);

    if (packet_response_error_type_ != PacketResponseErrorType::ERR_NONE)
    {
        throw std::runtime_error("Packet error response type");
    }
}


// -------- SearchTagPacket --------
SearchTagPacket::SearchTagPacket(BYTE max_id_bytes)
    : Packet(PacketType::SEARCH_TAG, COMMAND_NUMBER_SEARCH_TAG)
    , max_id_bytes_(max_id_bytes)
    , result_(false)
    , tag_type_(TagType::NOTAG)
    , id_bit_count_(0)
    , id_byte_count_(0)
{

}

string SearchTagPacket::GetData() const
{
    stringstream ss;

    ss << GetCommandNumber();
    ss << std::hex << std::setfill('0');
    ss << std::uppercase;
    ss << std::setw(2) << (max_id_bytes_ & 0xFF);
    ss << PACKET_CARRIEGE_RETURN;

    return ss.str();
}

void SearchTagPacket::ParseResponse(const vector<BYTE>& response)
{
    // Check the base of the response
    Packet::ParseResponse(response);

    // Response has to be atleast 2 bytes - NO TAG FOUND [0x0, 0x0]
    if (response.size() < 2 || response[0] != 0 || response[1] == 0 && response.size() != 2)
    {
        throw std::runtime_error("SearchTagPacket invalid response");
    }

    result_ = response[1];

    // NO TAG FOUND
    if (result_ == false)
        return;

    if (response.size() < 5)
    {
        throw std::runtime_error("SearchTagPacket invalid response - tag found");
    }

    tag_type_ = static_cast<TagType>(response[2]);
    id_bit_count_ = response[3];
    id_byte_count_ = response[4];

    if (response.size() < id_byte_count_ + 5)
    {
        throw std::runtime_error("SearchTagPacket invalid response - tag length");
    }

    vector<BYTE> id_bytes;
    id_bytes.assign(response.begin() + 5, response.end());

    id_ = Utilities::VectorToStringCompact(id_bytes);
}

BYTE SearchTagPacket::GetMaxIdBytes() const
{
    return max_id_bytes_;
}

bool SearchTagPacket::GetResult() const
{
    return result_;
}

TagType SearchTagPacket::GetTagType() const
{
    return tag_type_;
}

BYTE SearchTagPacket::GetIdBitCount() const
{
    return id_bit_count_;
}

BYTE SearchTagPacket::GetIdByteCount() const
{
    return id_byte_count_;
}

const string& SearchTagPacket::GetId() const
{
    return id_;
}


// -------- SetTagTypesPacket --------
SetTagTypesPacket::SetTagTypesPacket(TagType lf, TagType hf)
    : Packet(PacketType::SET_TAG_TYPES, COMMAND_NUMBER_SET_TAG_TYPES)
    , lf_(lf)
    , hf_(hf)
{

}

string SetTagTypesPacket::GetData() const
{
    stringstream ss;
    ss << GetCommandNumber() << std::hex << std::setfill('0');
    ss << std::uppercase;

    // LF
    ss << std::setw(2) << (static_cast<uint32_t>(lf_) & 0xFF);
    ss << std::setw(2) << ((static_cast<uint32_t>(lf_) & 0xFF00) >> 8);
    ss << std::setw(2) << ((static_cast<uint32_t>(lf_) & 0xFF0000) >> 16);
    ss << std::setw(2) << ((static_cast<uint32_t>(lf_) & 0xFF000000) >> 24);

    // HF
    ss << std::setw(2) << (static_cast<uint32_t>(hf_) & 0xFF);
    ss << std::setw(2) << ((static_cast<uint32_t>(hf_) & 0xFF00) >> 8);
    ss << std::setw(2) << ((static_cast<uint32_t>(hf_) & 0xFF0000) >> 16);
    ss << std::setw(2) << ((static_cast<uint32_t>(hf_) & 0xFF000000) >> 24);

    ss << PACKET_CARRIEGE_RETURN;

    return ss.str();
}

void SetTagTypesPacket::ParseResponse(const vector<BYTE>& response)
{
    // Check the base of the response
    Packet::ParseResponse(response);

    // Response contains only error byte
    if (response.size() != 1)
    {
        throw std::runtime_error("SetTagTypesPacket invalid response");
    }
}

TagType SetTagTypesPacket::GetLF() const
{
    return lf_;
}

TagType SetTagTypesPacket::GetHF() const
{
    return hf_;
}

// --------CheckPresencePacket--------
CheckPresencePacket::CheckPresencePacket()
    : Packet(PacketType::CHECK_PRESENCE, COMMAND_NUMBER_CHECK_PRESENCE)
    , result_(false)
{

}

string CheckPresencePacket::GetData() const
{
    stringstream ss;

    ss << std::uppercase;
    ss << GetCommandNumber();
    ss << PACKET_CARRIEGE_RETURN;

    return ss.str();
}

void CheckPresencePacket::ParseResponse(const vector<BYTE>& response)
{
    // Check the base of the response
    Packet::ParseResponse(response);

    // Response has to be exactly 2
    if (response.size() != 2)
    {
        throw std::runtime_error("CheckPresencePacket invalid response");
    }

    result_ = response[1];
}

bool CheckPresencePacket::GetResult() const
{
    return result_;
}


// -------- ISO14443_4_TDX_Packet --------
ISO14443_4_TDX_Packet::ISO14443_4_TDX_Packet(BYTE payload_size, const string& payload, BYTE maximum_response_size)
    : Packet(PacketType::ISO14443_4_TDX_Packet, COMMAND_NUMBER_ISO14443_4_TDX)
    , payload_size_(payload_size)
    , payload_(payload)
    , maximum_response_size_(maximum_response_size)
    , result_(false)
    , actual_response_size_(0)
{

}

ISO14443_4_TDX_Packet::ISO14443_4_TDX_Packet(const APDUCommand& apdu_command, BYTE maximum_response_size)
    : Packet(PacketType::ISO14443_4_TDX_Packet, COMMAND_NUMBER_ISO14443_4_TDX)
    , payload_size_(apdu_command.GetPayloadSize())
    , payload_(apdu_command.GetPayload())
    , maximum_response_size_(maximum_response_size)
    , result_(false)
    , actual_response_size_(0)
{

}

string ISO14443_4_TDX_Packet::GetData() const
{
    stringstream ss;

    ss << GetCommandNumber() << std::hex << std::setfill('0') << std::uppercase;
    ss << std::setw(2) << (payload_size_ & 0xFF);
    ss << payload_;
    ss << std::setw(2) << (maximum_response_size_ & 0xFF);

    ss << PACKET_CARRIEGE_RETURN;

    return ss.str();
}

void ISO14443_4_TDX_Packet::ParseResponse(const vector<BYTE>& response)
{
    // Check the base of the response
    Packet::ParseResponse(response);

    // Response has to be atleast 3
    if (response.size() < 3)
    {
        throw std::runtime_error("ISO14443_4_TDX_Packet invalid response");
    }

    result_ = response[1];
    actual_response_size_ = response[2];

    vector<BYTE> response_bytes;
    response_bytes.reserve(actual_response_size_);
    for (auto i = 0; i < actual_response_size_; i++)
    {
        response_bytes.emplace_back(response[i+3]);
    }

    response_ = Utilities::VectorToStringCompact(response_bytes);
}

BYTE ISO14443_4_TDX_Packet::GetPayloadSize() const
{
    return payload_size_;
}

const string& ISO14443_4_TDX_Packet::GetPayload() const
{
    return payload_;
}

BYTE ISO14443_4_TDX_Packet::GetMaximumResponseSize() const
{
    return maximum_response_size_;
}

bool ISO14443_4_TDX_Packet::GetResult() const
{
    return result_;
}

BYTE ISO14443_4_TDX_Packet::GetActualResponseSize() const
{
    return actual_response_size_;
}

const string& ISO14443_4_TDX_Packet::GetResponse() const
{
    return response_;
}