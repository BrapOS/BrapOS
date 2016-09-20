#include <stddef.h>
#include <stdint.h>

#include "util/util.hpp"
#include "Terminal.hpp"
#include "SerialPort.hpp"


size_t strlen(const char* str)
{
    size_t length = 0;
    while (str[length] != 0) {
        ++length;
    }

    return length;
}


extern "C"

void kernel_main()
{
    Terminal terminal;
    terminal.write("Welcome to BrapOS!\n");

    SerialPort com1(SerialPort::getAddress(1));
    com1.write("Serial port COM1 enabled...\n");
}