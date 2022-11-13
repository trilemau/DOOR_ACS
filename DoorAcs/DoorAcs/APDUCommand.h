#pragma once

#include "Libs.h"


// -------- APDUCommand --------
class APDUCommand
{
    APDUCommandType apdu_command_type_;
    BYTE cla_;
    BYTE ins_;
    BYTE parameter1_;
    BYTE parameter2_;
    BYTE data_length_;
    string data_;
    BYTE response_length_;

protected:
    void SetData(const string& data);

public:

    APDUCommand(APDUCommandType apdu_command_type, BYTE cla, BYTE parameter1, BYTE parameter2, BYTE response_length);

    virtual string GetPayload() const = 0;
    BYTE GetPayloadSize() const;
    string GetHeader() const;

    APDUCommandType GetAPDUCommandType() const;
    BYTE GetCla() const;
    BYTE GetIns() const;
    BYTE GetParameter1() const;
    BYTE GetParameter2() const;
    BYTE GetDataLength() const;
    const string& GetData() const;
    BYTE GetResponseLength() const;
};


// -------- SelectFileAPDUCommand --------
class SelectFileAPDUCommand : public APDUCommand
{
    string aid_;

public:
    SelectFileAPDUCommand(const string& aid);

    string GetPayload() const override;

    const string& GetAid() const;
};


// -------- GetResponseAPDUCommand --------
class GetResponseAPDUCommand : public APDUCommand
{
public:
    GetResponseAPDUCommand();
    
    string GetPayload() const override;
};