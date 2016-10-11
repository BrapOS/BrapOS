#include "KernelLogger.hpp"

/**
 * \brief Configure the kernel logger with a terminal and a serial port
 *
 * \param terminal The terminal used for output
 * \param serialPort The serial port used for output
 */
KernelLogger::KernelLogger(Terminal* terminal, SerialPort* serialPort)
    : terminal_(terminal),
      serialPort_(serialPort)
{
}

/**
 * \brief Print text on devices specified
 *
 * The text will be outputted on the devices, prefixed with "[KERNEL]".
 *
 * \param data A pointer to a null-terminated string. It should not end with
 * '\n'.
 */
void KernelLogger::log(const char* data)
{
    const char* prefix = "[KERNEL] ";
    const char* endLine = "\n";

    terminal_->write(prefix);
    terminal_->write(data);
    terminal_->write(endLine);

    serialPort_->write(prefix);
    serialPort_->write(data);
    serialPort_->write(endLine);
}