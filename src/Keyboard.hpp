#pragma once

#include <stddef.h>
#include <stdint.h>

class KeyboardEntry
{
public:
    KeyboardEntry() {};
    KeyboardEntry(unsigned char scancode,  bool isPressed,
                  bool isLeftShiftPressed, bool isRightShiftPressed,
                  bool isCtrlPressed,      bool isAltPressed);

    unsigned char getCharacter() const;
    bool isPressed() const;
    bool isLeftShiftPressed() const;
    bool isRightShiftPressed() const;
    bool isCtrlPressed() const;
    bool isAltPressed() const;

private:
    unsigned char character_;
    bool isPressed_;
    bool isLeftShiftPressed_;
    bool isRightShiftPressed_;
    bool isCtrlPressed_;
    bool isAltPressed_;

    static const unsigned char usMapping_[128];
};

class Keyboard
{
public:
    static Keyboard& getInstance();

    KeyboardEntry readEntry();
    void putEntry(const KeyboardEntry& entry);
    bool isEmpty() const;

    Keyboard(Keyboard const&) = delete;
    void operator=(Keyboard const&) = delete;

private:
    Keyboard();

    static Keyboard instance_;

    uint32_t readIndex_;
    uint32_t writeIndex_;
    static const uint32_t CAPACITY = 1024;
    KeyboardEntry buffer_[CAPACITY];
};
