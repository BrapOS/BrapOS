#pragma once

/// Load the interrupt descriptor table
void lidt(void *base, unsigned int limit);
/// Initialize the interrupt descriptor table
void initializeIdt();
/// Initialize the PIC
void configPIC();
