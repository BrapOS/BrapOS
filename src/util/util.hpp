#pragma once

#include <stddef.h>
#include <stdint.h>

namespace util
{
    /// Convert a number to an hexadecimal representation of the form 0xXXXXXXXX
    void convertToHexa(uint32_t number, char* output);
}