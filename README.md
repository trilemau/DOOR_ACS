# Access Control System NFC HCE

## Description
This is a simple access control system written in C++ that has one connected reader to it. The reader can read ID from access cards and NFC-enabled smartphones to evaluate access granted or access denied.

## Requirements
- Microsoft Visual Studio 2022 supporting C++17
- ASSET 622 reader connected using a USB RS232 converter

## Visual Studio Solution
The solution contains:
- the implementation of access control system that uses NFC host-based card emulation to authenticate a user
- unit tests

## To run:
Both the implementation and tests are run using Microsoft Visual Studio 2022. The ASSET 622 reader has to be connected to the computer using a USB RS232 converter prior to the execution of the program. It is imperative to set the value of macro COM_PORT found in the Constants.h file to the COM port where the reader is connected to.

After the start of the access control system, the reader should automatically start to constantly search for access cards or NFC-enabled smartphones. Important information about the program execution is printed in the application's console.

## Authors
Tri Le Mau
