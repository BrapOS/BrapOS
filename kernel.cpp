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


void outb(uint16_t port, uint8_t data)
{
    __asm__ (
        "outb %0, %1"
        :
        : "a"(data), "d"(port)
    );
}


namespace vga
{
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

    const size_t WIDTH  = 80;
    const size_t HEIGHT = 25;


    class Screen
    {
    public:
        Screen();

        uint16_t putEntryAt(char character, Color foregroundColor, Color backgroundColor, size_t x, size_t y);
        uint16_t putEntryAt(uint16_t entry, size_t x, size_t y);

        void putCursorAt(size_t x, size_t y);

    private:
        size_t convertPositionToIndex(size_t x, size_t y) const;
        uint8_t makeColor(Color foreground, Color background) const;
        uint16_t makeEntry(char character, Color foregroundColor, Color backgroundColor) const;

        uint16_t* buffer_;

        const uint16_t CURSOR_COMMAND_PORT = 0x3D4;
        const uint16_t CURSOR_DATA_PORT    = 0x3D5;

        const uint8_t CURSOR_HIGH_BYTE_COMMAND = 14;
        const uint8_t CURSOR_LOW_BYTE_COMMAND  = 15;
    };

    Screen::Screen()
        : buffer_((uint16_t*) 0xB8000)
    {
    }

    uint16_t Screen::putEntryAt(char character, Color foregroundColor, Color backgroundColor, size_t x, size_t y)
    {
        return putEntryAt( makeEntry(character, foregroundColor, backgroundColor), x, y );
    }

    uint16_t Screen::putEntryAt(uint16_t entry, size_t x, size_t y)
    {
        const size_t index = convertPositionToIndex(x, y);
        buffer_[index] = entry;
        return entry;
    }

    void Screen::putCursorAt(size_t x, size_t y)
    {
        size_t index = convertPositionToIndex(x, y);
        uint8_t lowByte  = index & 0xFF,
                highByte = (index >> 8) & 0xFF;

        outb(CURSOR_COMMAND_PORT, CURSOR_HIGH_BYTE_COMMAND);
        outb(CURSOR_DATA_PORT,    highByte);
        outb(CURSOR_COMMAND_PORT, CURSOR_LOW_BYTE_COMMAND);
        outb(CURSOR_DATA_PORT,    lowByte);
    }

    size_t Screen::convertPositionToIndex(size_t x, size_t y) const
    {
        return y * WIDTH + x;
    }

    uint8_t Screen::makeColor(Color foreground, Color background) const
    {
        return foreground | background << 4;
    }

    uint16_t Screen::makeEntry(char character, Color foregroundColor, Color backgroundColor) const
    {
        return static_cast<uint16_t>(character) | makeColor(foregroundColor, backgroundColor) << 8;
    }
}


class Terminal
{
public:
    Terminal();
    void write(const char* data);

private:
    void addLine();
    void putChar(char character);

    // Prochain caractère
    size_t      row_;
    size_t      column_;
    vga::Color  foregroundColor_;
    vga::Color  backgroundColor_;
    vga::Screen screen_;

    uint16_t    screenBuffer_[vga::HEIGHT][vga::WIDTH];

    const size_t LAST_ROW    = vga::HEIGHT - 1;
    const size_t LAST_COLUMN = vga::WIDTH  - 1;
};

Terminal::Terminal()
    : row_(0),
      column_(0),
      foregroundColor_(vga::COLOR_LIGHT_GREY),
      backgroundColor_(vga::COLOR_BLACK)
{
    for (size_t row = 0; row < vga::HEIGHT; ++row) {
        for (size_t column = 0; column < vga::WIDTH; ++column) {
            screenBuffer_[row][column] = screen_.putEntryAt(' ', foregroundColor_, backgroundColor_, column, row);
        }
    }
}

void Terminal::write(const char* data)
{
    for (size_t i = 0; data[i] != '\0'; ++i) {
        putChar(data[i]);
    }
}

void Terminal::addLine()
{
    if (row_ < LAST_ROW) {
        ++row_;
    }
    else {
        for (size_t row = 1; row < vga::HEIGHT; ++row) {
            for (size_t column = 0; column < vga::WIDTH; ++column) {
                size_t precedingRow = row - 1;
                uint16_t entry = screenBuffer_[precedingRow][column] = screenBuffer_[row][column];
                screen_.putEntryAt(entry, column, precedingRow);
            }
        }
        for (size_t column = 0; column < vga::WIDTH; ++column) {
            screenBuffer_[LAST_ROW][column] = screen_.putEntryAt(' ', foregroundColor_, backgroundColor_, column, LAST_ROW);
        }
    }

    column_ = 0;
}

void Terminal::putChar(char character)
{
    switch (character) {
        case '\n' :
            addLine();
            screen_.putCursorAt(column_, row_);
            break;

        default :
            screenBuffer_[row_][column_] = screen_.putEntryAt(character, foregroundColor_, backgroundColor_, column_, row_);
            ++column_;
            if (column_ > LAST_COLUMN) {
                addLine();
            }
            screen_.putCursorAt(column_, row_);
    }
}


extern "C"

void kernel_main()
{
    Terminal terminal;
    terminal.write("Hello, kernel World!\n");
    terminal.write("Such a nice day!\n");
    terminal.write("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec pellentesque mole\n");
}