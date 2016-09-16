#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * \brief Wrap the constants and the functions needed to write to the
 * framebuffer (used to manipulate the screen)
 *
 * VGA stands for _Video Graphics Array_.
 */
namespace vga
{
    /**
     * \brief List of displayable colors
     *
     * This can be used for the background and the foreground.
     */
    enum Color : uint8_t
    {
        COLOR_BLACK         = 0,
        COLOR_BLUE          = 1,
        COLOR_GREEN         = 2,
        COLOR_CYAN          = 3,
        COLOR_RED           = 4,
        COLOR_MAGENTA       = 5,
        COLOR_BROWN         = 6,
        COLOR_LIGHT_GREY    = 7,
        COLOR_DARK_GREY     = 8,
        COLOR_LIGHT_BLUE    = 9,
        COLOR_LIGHT_GREEN   = 10,
        COLOR_LIGHT_CYAN    = 11,
        COLOR_LIGHT_RED     = 12,
        COLOR_LIGHT_MAGENTA = 13,
        COLOR_LIGHT_BROWN   = 14,
        COLOR_WHITE         = 15
    };

    /// Maximum width of the screen
    const size_t WIDTH  = 80;
    /// Maximum height of the screen
    const size_t HEIGHT = 25;
}