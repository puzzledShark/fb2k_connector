#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <Windows.h>

class SerialPort {
public:
    SerialPort(const std::string& portName);
    ~SerialPort();
    std::string read();

private:
    HANDLE hSerial;
};

#endif // SERIALPORT_H
