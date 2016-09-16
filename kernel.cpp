#include <stddef.h>
#include <stdint.h>


size_t strlen(const char* str)
{
    size_t length = 0;
    while (str[length] != 0) {
        ++length;
    }

    return length;
}


/**
 * \brief Wrap the `inb` assembly instruction to read a byte from an I/O port
 *
 * \param port The I/O port to read from
 * \return The value read from the I/O port
 */
uint8_t inb(uint16_t port)
{
    uint8_t data = 0;

    __asm__ (
        "inb %1, %0"
        : "=a"(data)
        : "d"(port)
    );

    return data;
}


/**
 * \brief Wrap the `outb` assembly instruction to write a byte to an I/O port
 *
 * \param port The I/O port to write to
 * \param data The byte to write to the I/O port
 */
void outb(uint16_t port, uint8_t data)
{
    __asm__ (
        "outb %0, %1"
        :
        : "a"(data), "d"(port)
    );
}

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


extern "C"

void kernel_main()
{
    Terminal terminal;
    terminal.write("Hello, kernel World!\n");
    terminal.write("Such a nice day!\n");
    terminal.write("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec pellentesque mole\n");
}