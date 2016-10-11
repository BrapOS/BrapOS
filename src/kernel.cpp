#include <stddef.h>
#include <stdint.h>

#include "util/util.hpp"
#include "Terminal.hpp"
#include "SerialPort.hpp"
#include "interrupt.hpp"
#include "Keyboard.hpp"
#include "KernelLogger.hpp"

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
    // Initialize the terminal and COM1 serial port
    Terminal terminal;
    SerialPort com1(SerialPort::getAddress(1));

    // Create the kernel logger
    KernelLogger logger(&terminal, &com1);

    logger.log("Serial port COM1 enabled");

    // Inialize interruptions and PIC
    initializeIdt();
    logger.log("IDT loaded");
    configPIC();
    logger.log("PIC configured");
    __asm__ ("sti");

    // Greet the user
    terminal.write("Welcome to BrapOS!\n");

    // Write what the user types on the terminal
    while (true) {
        while (!Keyboard::getInstance().isEmpty()) {
            KeyboardEntry entry = Keyboard::getInstance().readEntry();
            if (entry.isPressed()) {
                unsigned char data[2] = {entry.getCharacter(), '\0'};
                terminal.write(data);
            }
        }

        __asm__ ("hlt");
    }
}