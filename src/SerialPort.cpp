#include "SerialPort.hpp"

const uint16_t* SerialPort::biosDataAreaAddress_ = (uint16_t*) 0x400;

/**
 * \brief Configure the serial port at address `address`
 *
 * \param address : The base address of the serial port
 */
SerialPort::SerialPort(uint16_t address)
    : dataPort_(address),
      fifoCommandPort_(address + 2),
      lineCommandPort_(address + 3),
      modemCommandPort_(address + 4),
      lineStatusPort_(address + 5)
{
    uint32_t divisor = 1;
    uint8_t divisorLowByte  = divisor & 0xFF,
            divisorHighByte = (divisor >> 8) & 0xFF;

    // Configure the baud rate
    outb(lineCommandPort_, 0x80); // Enable DLAB
    outb(dataPort_, divisorHighByte);
    outb(dataPort_, divisorLowByte);

    // Configure the line
    outb(lineCommandPort_, 0x03);

    // Configure the buffer
    outb(fifoCommandPort_, 0xC7);

    // Configure the modem
    outb(modemCommandPort_, 0x03);
}

/**
 * \brief Send a string of characters to the serial port
 *
 * This method takes a pointer to a null-terminated string and will send each
 * character until it meets the null character '\0'.
 *
 * \param data A pointer to a null-terminated string
 */
void SerialPort::write(const char* data)
{
    // Send each character over the serial line
    for (size_t i = 0; data[i] != '\0'; ++i) {
        // Wait for the queue to be empty before sending it
        while (!isTransmitFifoEmpty())
            ;

        outb(dataPort_, data[i]);
    }
}

/**
 * \brief Get the address of a serial port (COM port)
 *
 * \param comPort The COM port (beginning at 1)
 * \return The base address of the serial port
 */
uint16_t SerialPort::getAddress(uint8_t comPort)
{
    return biosDataAreaAddress_[comPort - 1];
}

/**
 * \brief Check if the queue is empty
 *
 * \return 0 if the queue is not empty, a positive number ortherwise
 */
uint8_t SerialPort::isTransmitFifoEmpty()
{
    return inb(lineStatusPort_) & 0x20;
}