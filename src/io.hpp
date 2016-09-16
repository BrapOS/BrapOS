#pragma once

#include <stddef.h>
#include <stdint.h>

/// Wrap the `inb` assembly instruction to read a byte from an I/O port
uint8_t inb(uint16_t port);

/// Wrap the `outb` assembly instruction to write a byte to an I/O port
void outb(uint16_t port, uint8_t data);