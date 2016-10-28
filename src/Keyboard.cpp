#include "Keyboard.hpp"

const unsigned char KeyboardEntry::usMapping_[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`',   0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/',   0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

KeyboardEntry::KeyboardEntry(unsigned char scancode,  bool isPressed,
                             bool isLeftShiftPressed, bool isRightShiftPressed,
                             bool isCtrlPressed,      bool isAltPressed)
    : isPressed_(isPressed),
      isLeftShiftPressed_(isLeftShiftPressed),
      isRightShiftPressed_(isRightShiftPressed),
      isCtrlPressed_(isCtrlPressed), isAltPressed_(isAltPressed)
{
    character_ = usMapping_[scancode];
}

unsigned char KeyboardEntry::getCharacter() const
{
    return character_;
}

bool KeyboardEntry::isPressed() const
{
    return isPressed_;
}

bool KeyboardEntry::isLeftShiftPressed() const
{
    return isLeftShiftPressed_;
}

bool KeyboardEntry::isRightShiftPressed() const
{
    return isRightShiftPressed_;
}

bool KeyboardEntry::isCtrlPressed() const
{
    return isCtrlPressed_;
}

bool KeyboardEntry::isAltPressed() const
{
    return isAltPressed_;
}

Keyboard Keyboard::instance_;

Keyboard::Keyboard()
    : readIndex_(0), writeIndex_(0)
{
}

Keyboard& Keyboard::getInstance()
{
    return instance_;
}

KeyboardEntry Keyboard::readEntry()
{
    KeyboardEntry entry = buffer_[readIndex_];
    readIndex_ = (readIndex_ + 1) % CAPACITY;
    return entry;
}

void Keyboard::putEntry(const KeyboardEntry& entry)
{
    buffer_[writeIndex_] = entry;
    writeIndex_ = (writeIndex_ + 1) % CAPACITY;
}

bool Keyboard::isEmpty() const
{
    return readIndex_ == writeIndex_;
}
