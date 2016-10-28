#include "Terminal.hpp"

/**
 * \brief Initialize the terminal object by clearing the screen
 *
 * The constructor initializes the cursor position to (0,0). It then sets the
 * foreground color to a light grey and the background color black. Finally, it
 * clears the screen.
 */
Terminal::Terminal()
    : row_(0),
      column_(0),
      foregroundColor_(vga::COLOR_LIGHT_GREY),
      backgroundColor_(vga::COLOR_BLACK)
{
    // Clear the terminal by putting spaces at every position of the screen
    // buffer
    for (size_t row = 0; row < vga::HEIGHT; ++row) {
        for (size_t column = 0; column < vga::WIDTH; ++column) {
            screenBuffer_[row][column] = screen_.putEntryAt(
                ' ', foregroundColor_, backgroundColor_, column, row);
        }
    }
}

/**
 * \brief Write a string of characters to the terminal
 *
 * This method takes a pointer to a null-terminated string and will print each
 * character until it meets the null character '\0'. Text will automatically
 * wrap at column 80 and scroll at the end of the screen.
 *
 * \param data A pointer to a null-terminated string
 */
void Terminal::write(const char* data)
{
    // Write each character of the string until the null character '\0' is
    // encountered
    for (size_t i = 0; data[i] != '\0'; ++i) {
        putChar(data[i]);
    }
}

void Terminal::write(const unsigned char* data) {
    for (size_t i = 0; data[i] != '\0'; ++i) {
        putChar(data[i]);
    }
}

/**
 * \brief Add an empty line (and scroll the screen one row if there is no space
 * left)
 *
 * This method moves the cursor down one line if there is enough space. If not,
 * the content is scrolled up one line to make room.
 */
void Terminal::addLine()
{
    // Move the cursor to the next line if the cursor is not on the last row
    if (row_ < LAST_ROW) {
        ++row_;
    }
    // Scroll the content of the screen one line up (and discard the very first
    // row) to make room for the new content
    else {
      // Move all lines to the preceding line in the buffer and on the screen
      for (size_t row = 1; row < vga::HEIGHT; ++row) {
        for (size_t column = 0; column < vga::WIDTH; ++column) {
          size_t precedingRow = row - 1;
          uint16_t entry = screenBuffer_[precedingRow][column] =
              screenBuffer_[row][column];
          screen_.putEntryAt(entry, column, precedingRow);
        }
      }
      // Reinitialize the last row of the screen by putting spaces
      for (size_t column = 0; column < vga::WIDTH; ++column) {
        screenBuffer_[LAST_ROW][column] = screen_.putEntryAt(
            ' ', foregroundColor_, backgroundColor_, column, LAST_ROW);
      }
    }

    column_ = 0;
}

/**
 * \brief Put a character at the position of the cursor
 *
 * This method takes a character. If the character is the newline character
 * '\n', it adds a line. If the character is anything else, it adds that
 * character to the screen buffer. The cursor is then placed after the last
 * character (or at the beginning of the next line if the end of the row was
 * reached).
 *
 * \param character The ASCII code of the character to be displayed
 */
void Terminal::putChar(char character)
{
    switch (character) {
        // Treat the newline character by adding a line
        case '\n' :
            addLine();
            break;

        default :
            // Put any other character on the screen and save the result for
            // future use (when scrolling the content)
            screenBuffer_[row_][column_] = screen_.putEntryAt(
                character, foregroundColor_, backgroundColor_, column_, row_);

            // Increment the cursor position and add a line if it is past the
            // end of the line
            ++column_;
            if (column_ > LAST_COLUMN) {
                addLine();
            }
    }

    // Move the cursor
    screen_.putCursorAt(column_, row_);
}
