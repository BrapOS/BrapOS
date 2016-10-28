#include "Terminal.hpp"
#include "io.hpp"
#include "SerialPort.hpp"
#include "interrupt.hpp"
#include "Keyboard.hpp"

extern "C" void handleInterruptKeyboard();

/// The interrupt descriptor table (initialized with zeros)
uint64_t idt[256] = {};

/**
 * \brief Load interrupt descriptor table entry
 *
 */
void lidt(void *base, unsigned int limit)
{
    unsigned int i[2];

    i[0] = limit << 16;
    i[1] = (unsigned int) base;
    asm ("lidt (%0)": :"r" (((char*) i) + 2));
}

void initializeIdt()
{
    // Keyboard interruption
    uint64_t addressKeyboard = (uint32_t) &handleInterruptKeyboard;
    idt[33] = 0x00008E0000080000;
    idt[33] |= addressKeyboard & 0xFFFF;
    idt[33] |= (addressKeyboard & 0xFFFF0000) << 32;

    lidt(idt, 256*8);
}

/**
 * \brief Initialize the PIC
 *
 */
void configPIC()
{
    // Initialize the ICW1
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // Initialize the ICW2
    outb(0x21, 0x20);
    outb(0xA1, 0x70);

    // Initialize the ICW3
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // Initialize the ICW4
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Only listen to irqs 0, 1, and 2
    outb(0x21,0xfd);
    outb(0xa1,0xff);
}

/**
 * \brief Keyboard interrupt handler
 *
 * Interrupt service routine that is called when a keyboard key is pressed or
 * released.
 */
extern "C" void cHandleInterruptKeyboard()
{
    static bool leftShiftPressed  = false;
    static bool rightShiftPressed = false;
    static bool controlPressed    = false;
    static bool altPressed        = false;

    // Read from the keyboard's data buffer
    unsigned char scancode = inb(0x60);

    // Key released
    bool isPressed = !(scancode & 0x80);
    if (!isPressed) {
        scancode -= 0x80;
        switch (scancode) {
            case 0x2A:
                leftShiftPressed = false;
                break;
            case 0x36:
                rightShiftPressed = false;
                break;
            case 0x38:
                altPressed = false;
                break;
            case 0x1D:
                controlPressed = false;
                break;
        }
    }
    else { // Key pressed
        switch (scancode) {
            case 0x2A:
                leftShiftPressed = true;
                break;
            case 0x36:
                rightShiftPressed = true;
                break;
            case 0x38:
                altPressed = true;
                break;
            case 0x1D:
                controlPressed = true;
                break;
        }
    }

    Keyboard::getInstance().putEntry(KeyboardEntry(
        scancode,
        isPressed,
        leftShiftPressed,
        rightShiftPressed,
        controlPressed,
        altPressed
    ));

    // Send EOI to both master and slave
    outb(0x20,0x20);
    outb(0xa0,0x20);
}
