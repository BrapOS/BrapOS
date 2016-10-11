#pragma once

#include "Terminal.hpp"
#include "SerialPort.hpp"

/**
 * \brief Log kernel actions
 *
 * This object is used when the kernel wants to log information. This
 * information can be sent to the terminal and over the serial port.
 */
class KernelLogger
{
public:
    /// Configure the kernel logger with a terminal and a serial port
    KernelLogger(Terminal* terminal, SerialPort* serialPort);

    /// Print text on devices specified
    void log(const char* data);

private:
    /// The terminal used for output
    Terminal* terminal_;
    /// The serial port used for output
    SerialPort* serialPort_;
};