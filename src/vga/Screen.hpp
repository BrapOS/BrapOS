#pragma once

#include "vga.hpp"

namespace vga
{
    /**
     * \brief Display characters on screen and change the cursor position
     *
     * This class is a little abstraction over the screen. It can display a
     * character (with a foreground and background color) anywhere on the
     * screen. It can also places the cursor at any given location. It has no
     * memory of previous operations done with it.
     *
     * The method `putEntryAt` is used to display a character on the screen. It
     * will return the resulting formatted character, as expected by VGA. This
     * result can be saved and then given later to `putEntryAt` to implement
     * scrolling, for example.
     */
    class Screen
    {
    public:
        /// Initialize the screen object
        Screen();

        /// Put a character on the screen with a foreground and background color
        /// at a specified position
        uint16_t putEntryAt(char character, Color foregroundColor,
                            Color backgroundColor, size_t x, size_t y);
        /// Put a formatted character on the screen at a specified position
        uint16_t putEntryAt(uint16_t entry, size_t x, size_t y);

        /// Put the cursor at the specified position
        void putCursorAt(size_t x, size_t y);

    private:
        /// Convert a position on the screen in terms of x and y to an index
        size_t convertPositionToIndex(size_t x, size_t y) const;
        /// Construct the VGA byte color from a foreground and a background
        /// color
        uint8_t makeColor(Color foreground, Color background) const;
        /// Construct a formatted character (a two bytes' VGA entry) from a
        /// character, a foreground and a background color
        uint16_t makeEntry(char character, Color foregroundColor,
                           Color backgroundColor) const;

        /// Pointer to the framebuffer in memory
        uint16_t* buffer_;

        /// Port to send a command for the cursor
        const uint16_t CURSOR_COMMAND_PORT = 0x3D4;
        /// Port to send data for the cursor
        const uint16_t CURSOR_DATA_PORT    = 0x3D5;

        /// Tell the framebuffer that the next byte will be the highest 8 bits
        /// of the cursor position
        const uint8_t CURSOR_HIGH_BYTE_COMMAND = 14;
        /// Tell the framebuffer that the next byte will be the lowest 8 bits of
        /// the cursor position
        const uint8_t CURSOR_LOW_BYTE_COMMAND  = 15;
    };
}