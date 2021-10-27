#pragma once

#define TAGMASK(Tagtype)    static_cast<TagType>(1 << (static_cast<int>(Tagtype) & 0x1F))

enum class TagType
{
    NOTAG = 0,

    // LF Tags
    LFTAG_EM4102      = 0x40,    // "EM4x02/CASI-RUSCO" (aka IDRO_A)
    LFTAG_HITAG1S     = 0x41,    // "HITAG 1/HITAG S"   (aka IDRW_B)
    LFTAG_HITAG2      = 0x42,    // "HITAG 2"           (aka IDRW_C)
    LFTAG_EM4150      = 0x43,    // "EM4x50"            (aka IDRW_D)
    LFTAG_AT5555      = 0x44,    // "T55x7"             (aka IDRW_E)
    LFTAG_ISOFDX      = 0x45,    // "ISO FDX-B"         (aka IDRO_G)
    LFTAG_EM4026      = 0x46,    // N/A                 (aka IDRO_H)
    LFTAG_HITAGU      = 0x47,    // N/A                 (aka IDRW_I)
    LFTAG_EM4305      = 0x48,    // N/A                 (aka IDRW_K)
    LFTAG_HIDPROX     = 0x49,	// "HID Prox"
    LFTAG_TIRIS       = 0x4A,	// "ISO HDX/TIRIS"
    LFTAG_COTAG       = 0x4B,	// "Cotag"
    LFTAG_IOPROX      = 0x4C,	// "ioProx"
    LFTAG_INDITAG     = 0x4D,	// "Indala"
    LFTAG_HONEYTAG    = 0x4E,	// "NexWatch"
    LFTAG_AWID        = 0x4F,	// "AWID"
    LFTAG_GPROX       = 0x50,	// "G-Prox"
    LFTAG_PYRAMID     = 0x51,	// "Pyramid"
    LFTAG_KERI        = 0x52,	// "Keri"
    LFTAG_DEISTER	  = 0x53,	// "Deister"
    LFTAG_CARDAX	  = 0x54,	// "Cardax"
    LFTAG_NEDAP		  = 0x55,	// "Nedap"
    LFTAG_PAC		  = 0x56,	// "PAC"
    LFTAG_IDTECK	  = 0x57,	// "IDTECK"
    LFTAG_ULTRAPROX	  = 0x58,	// "UltraProx"
    LFTAG_ICT		  = 0x59,	// "ICT"
    LFTAG_ISONAS	  = 0x5A,	// "Isonas"

    // HF Tags
    HFTAG_MIFARE      = 0x80,	// "ISO14443A/MIFARE"
    HFTAG_ISO14443B   = 0x81,	// "ISO14443B"
    HFTAG_ISO15693    = 0x82,	// "ISO15693"
    HFTAG_LEGIC       = 0x83,	// "LEGIC"
    HFTAG_HIDICLASS   = 0x84,	// "HID iCLASS"
    HFTAG_FELICA      = 0x85,	// "FeliCa"
    HFTAG_SRX         = 0x86,	// "SRX"
    HFTAG_NFCP2P      = 0x87,	// "NFC Peer-to-Peer"
    HFTAG_BLE         = 0x88,	// "Bluetooth Low Energy"
    HFTAG_TOPAZ       = 0x89,   // "Topaz"
    HFTAG_CTS         = 0x8A,   // "CTS256 / CTS512"

    ALL_TAGS = -1 // 0xFFFFFFFF
};