/*
* These are experiments i did before dropping them
* what this function basically did was intercept the serial connections and process them directly in foobar2000 without the middle man
*/

#include "stdafx.h"
#include "SerialPort.h"
#include <iostream>
#include <Windows.h>
#include <stdexcept>

SerialPort::SerialPort(const std::string& portName) : hSerial(INVALID_HANDLE_VALUE) {
    std::wstring widePortName(portName.begin(), portName.end());
    hSerial = CreateFile(widePortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Error opening serial port");
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("Error getting serial port state");
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("Error setting serial port parameters");
    }
}

SerialPort::~SerialPort() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

std::string SerialPort::read() {
    DWORD bytesRead;
    char buffer[1024];
    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
        throw std::runtime_error("Error reading from serial port");
    }
    return std::string(buffer, bytesRead);
}
