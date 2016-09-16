#include <stddef.h>
#include <stdint.h>

#include "Terminal.hpp"


size_t strlen(const char* str)
{
    size_t length = 0;
    while (str[length] != 0) {
        ++length;
    }

    return length;
}


extern "C"

void kernel_main()
{
    Terminal terminal;
    terminal.write("Hello, kernel World!\n");
    terminal.write("Such a nice day!\n");
    terminal.write("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec pellentesque mole\n");
}