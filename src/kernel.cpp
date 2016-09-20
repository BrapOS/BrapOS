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
    terminal.write("Hello, kernel World!\n");
    terminal.write("Such a nice day!\n");
    terminal.write("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec pellentesque mole\n");

    uint32_t com1Address = SerialPort::getAddress(1);
    uint32_t com2Address = SerialPort::getAddress(2);
    uint32_t com3Address = SerialPort::getAddress(3);
    uint32_t com4Address = SerialPort::getAddress(4);

    SerialPort com1(com1Address), com2(com2Address), com3(com3Address), com4(com4Address);
    com1.write("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec pellentesque molestie justo. Integer venenatis sapien vitae elit mollis, ut volutpat elit tincidunt. Pellentesque non arcu id nunc rutrum vulputate. Phasellus et libero dui. Sed consectetur, tortor vitae lacinia dictum, arcu neque vulputate dui, scelerisque dignissim est leo et magna. Sed sem diam, mattis sed consectetur in, aliquet sed tellus. Integer vehicula ut odio id pulvinar. Nullam sed sodales tortor. In pretium nibh orci, sed porta nisi facilisis et. Duis porttitor libero eu dolor pharetra dignissim. Vivamus id nisl nisi. Aliquam vel ante in libero blandit maximus id ac urna. Donec et pellentesque magna.\n");

    char hexaAddress[11] = { '\0' };

    util::convertToHexa(com1Address, hexaAddress);
    com1.write(hexaAddress);
    util::convertToHexa(com2Address, hexaAddress);
    com1.write(hexaAddress);
    util::convertToHexa(com3Address, hexaAddress);
    com1.write(hexaAddress);
    util::convertToHexa(com4Address, hexaAddress);
    com1.write(hexaAddress);
}