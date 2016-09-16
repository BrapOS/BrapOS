#include "io.hpp"

/**
 * \brief Wrap the `inb` assembly instruction to read a byte from an I/O port
 *
 * \param port The I/O port to read from
 * \return The value read from the I/O port
 */
uint8_t inb(uint16_t port)
{
    uint8_t data = 0;

    __asm__ (
        "inb %1, %0"
        : "=a"(data)
        : "d"(port)
    );

    return data;
}


/**
 * \brief Wrap the `outb` assembly instruction to write a byte to an I/O port
 *
 * \param port The I/O port to write to
 * \param data The byte to write to the I/O port
 */
void outb(uint16_t port, uint8_t data)
{
    __asm__ (
        "outb %0, %1"
        :
        : "a"(data), "d"(port)
    );
}