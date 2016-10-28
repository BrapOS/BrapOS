#pragma once

#include "vga/Screen.hpp"

/**
 * \brief Display strings of characters on screen
 *
 * This object takes the total control of the screen. It is one level of
 * abstraction higher than vga::Screen. It reinitializes the screen at the
 * beginning. After, it is possible to write text on it. Text wraps
 * automatically at 80 columns (but not at word boundaries) and scrolls
 * automatically when there is no space left on the screen. The cursor is
 * positionned after the last character each time something is written on the
 * screen.
 */
class Terminal
{
public:
    /// Initialize the terminal object by clearing the screen
    Terminal();
    /// Write a string of characters to the terminal
    void write(const char* data);
    void write(const unsigned char* data);

private:
    /// Add an empty line (and scroll the screen one row if there is no space
    /// left)
    void addLine();
    /// Put a character at the position of the cursor
    void putChar(char character);

    /// The current row of the cursor
    size_t      row_;
    /// The current column of the cursor
    size_t      column_;
    /// The foreground color of the terminal
    vga::Color  foregroundColor_;
    /// The background color of the terminal
    vga::Color  backgroundColor_;
    /// The screen driver to actually put characters on the screen
    vga::Screen screen_;

    /// Copy of the content of the screen (to allow scrolling)
    uint16_t    screenBuffer_[vga::HEIGHT][vga::WIDTH];

    /// The last row index
    const size_t LAST_ROW    = vga::HEIGHT - 1;
    /// The last column index
    const size_t LAST_COLUMN = vga::WIDTH  - 1;
};
