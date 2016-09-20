#include "util.hpp"

namespace util
{
    /**
     * \brief Convert a number to an hexadecimal representation of the form
     * 0xXXXXXXXX
     *
     * \param number The number to convert
     * \param output The pointer to a C-string of size 11 to put the hexadecimal
     * representation
     */
    void convertToHexa(uint32_t number, char* output)
    {
        const char* HEXA_SYMBOLS = "0123456789ABCDEF";
        // Beginning of the representation
        output[0] = '0';
        output[1] = 'x';

        const size_t nbHexaCharacters = sizeof(number) * 2;
        for (size_t i = 0; i < nbHexaCharacters; ++i) {
            // Extract the 4 bits representing the hexadecimal character
            uint8_t nibble = (number >> (4*i)) & 0xF;

            // Put the corresponding character in the output
            // The rightmost character in the output is the four lowest bits in
            // the number
            output[nbHexaCharacters + 1 - i] = HEXA_SYMBOLS[nibble];
        }

        // End of C-string
        output[10] = '\0';
    }
}