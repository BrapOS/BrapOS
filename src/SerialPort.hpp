#pragma once

#include "io.hpp"

/**
 * \brief Send strings of characters to a serial port
 *
 * This object configures a serial port to allow output. It can be used for
 * debugging purposes and display on the host machine.
 *
 * Example:
 * \code
 * // Find the address of first the serial port
 * uint32_t com1Address = SerialPort::getAddress(1);
 * // Send a string to the serial port
 * com1.write("Hello!\n");
 * \endcode
 */
class SerialPort
{
public:
    /// Configure the serial port at address `address`
    SerialPort(uint16_t address);
    /// Send a string of characters to the serial port
    void write(const char* data);

    /// Get the address of a serial port (COM port)
    static uint16_t getAddress(uint8_t comPort);

private:
    /// Check if the queue is empty
    uint8_t isTransmitFifoEmpty();

    /// The base address for serial ports registers
    const uint16_t dataPort_;
    /// The register for the queue
    const uint16_t fifoCommandPort_;
    /// The register for the line
    const uint16_t lineCommandPort_;
    /// The register for the modem
    const uint16_t modemCommandPort_;
    /// The register for the line status
    const uint16_t lineStatusPort_;

    /// The base address of the BIOS data area (to access the addresses of the
    /// serial ports)
    static const uint16_t* biosDataAreaAddress_;
};