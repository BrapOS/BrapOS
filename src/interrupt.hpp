#pragma once

void lidt(void *base, unsigned int limit);
void initializeIdt();
void configPIC();
