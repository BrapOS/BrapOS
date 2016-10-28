#include <stddef.h>
#include <stdint.h>

#include "util/util.hpp"
#include "Terminal.hpp"
#include "SerialPort.hpp"
#include "interrupt.hpp"
#include "Keyboard.hpp"

size_t strlen(const char* str)
{
    size_t length = 0;
    while (str[length] != 0) {
        ++length;
    }

    return length;
}

extern "C" void kernel_main()
{
    Terminal terminal;
    terminal.write("Welcome to BrapOS!\n");

    SerialPort com1(SerialPort::getAddress(1));
    com1.write("Serial port COM1 enabled...\n");

    initializeIdt();

    // Pic initialization
    configPIC();

    // This is to enable the the interruption
    asm ("sti");

    while (true) {
        while (!Keyboard::getInstance().isEmpty()) {
            KeyboardEntry entry = Keyboard::getInstance().readEntry();
            if (entry.isPressed()) {
                unsigned char data[2] = {entry.getCharacter(), '\0'};
                terminal.write(data);
            }
        }
        asm("hlt");
    }
}
