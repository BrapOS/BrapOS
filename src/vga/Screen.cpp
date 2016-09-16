#include "Screen.hpp"
#include "../io.hpp"

namespace vga
{
    /**
     * \brief Initialize the screen object
     *
     * The constructor initializes the `buffer_` pointer to the starting
     * address of the memory-mapped I/O of the framebuffer.
     */
    Screen::Screen()
        : buffer_((uint16_t*) 0xB8000)
    {
    }

    /**
     * \brief Put a character on the screen with a foreground and background
     * color at a specified position
     *
     * \param character The character to put on the screen
     * \param foregroundColor The color of the character
     * \param backgroundColor The background color of the character
     * \param x The displacement from the left side of the screen (in terms of
     * characters), between 0 and WIDTH - 1
     * \param y The displacement from the top of the screen (in terms of
     * characters), between 0 and HEIGHT - 1
     * \return The formatted character expected by VGA (character, foreground
     * and background colors)
     */
    uint16_t Screen::putEntryAt(char character, Color foregroundColor,
                                Color backgroundColor, size_t x, size_t y) {
      return putEntryAt(makeEntry(character, foregroundColor, backgroundColor),
                        x, y);
    }

    /**
     * \brief Put a formatted character on the screen at a specified position
     *
     * This method can be used to display a character that has already been
     * displayed.
     *
     * \param entry The formatted character expected by VGA (character,
     * foreground and background colors), as returned by the fisrt version of
     * `putEntryAt`
     * \param x The displacement from the left side of the screen (in terms of
     * characters), between 0 and WIDTH - 1
     * \param y The displacement from the top of the screen (in terms of
     * characters), between 0 and HEIGHT - 1
     */
    uint16_t Screen::putEntryAt(uint16_t entry, size_t x, size_t y)
    {
        const size_t index = convertPositionToIndex(x, y);

        // Put the formatted character in the framebuffer. The screen works with
        // memory-mapped I/O and will then be updated.
        buffer_[index] = entry;

        return entry;
    }

    /**
     * \brief Put the cursor at the specified position
     *
     * \param x The displacement from the left side of the screen (in terms of
     * characters), between 0 and WIDTH - 1
     * \param y The displacement from the top of the screen (in terms of
     * characters), between 0 and HEIGHT - 1
     */
    void Screen::putCursorAt(size_t x, size_t y)
    {
        size_t index = convertPositionToIndex(x, y);
        uint8_t lowByte  = index & 0xFF,
                highByte = (index >> 8) & 0xFF;

        // Send the cursor position: the 8 highest bits, then the 8 lowest bits
        outb(CURSOR_COMMAND_PORT, CURSOR_HIGH_BYTE_COMMAND);
        outb(CURSOR_DATA_PORT,    highByte);
        outb(CURSOR_COMMAND_PORT, CURSOR_LOW_BYTE_COMMAND);
        outb(CURSOR_DATA_PORT,    lowByte);
    }

    /**
     * \brief Convert a position on the screen in terms of x and y to an index
     *
     * \param x The displacement from the left side of the screen (in terms of
     * characters), between 0 and WIDTH - 1
     * \param y The displacement from the top of the screen (in terms of
     * characters), between 0 and HEIGHT - 1
     * \return The memory displacement to access the entry at (x,y)
     */
    size_t Screen::convertPositionToIndex(size_t x, size_t y) const
    {
        return y * WIDTH + x;
    }

    /**
     * \brief Construct the VGA byte color from a foreground and a background
     * color
     *
     * VGA expects the foreground to be the lowest 4 bits of the byte color and
     * the background color the be the highest 4 bits.
     *
     * \param foreground The foreground color of a character
     * \param background The background color of a character
     * \return A byte containing the foreground and the background color
     */
    uint8_t Screen::makeColor(Color foreground, Color background) const
    {
        return foreground | background << 4;
    }

    /**
     * \brief Construct a formatted character (a two bytes' VGA entry) from a
     * character, a foreground and a background color
     *
     * VGA can display a character with a color (the foreground one) on a
     * background color. The lowest byte is the ASCII code of the character and
     * the highest one is the colors.
     *
     * \param character The ASCII code of the character to be displayed
     * \param foregroundColor The color of the character
     * \param backgroundColor The background color of the character
     * \return The formatted character
     */
    uint16_t Screen::makeEntry(char character, Color foregroundColor,
                               Color backgroundColor) const {
      return static_cast<uint16_t>(character) |
             makeColor(foregroundColor, backgroundColor) << 8;
    }
}