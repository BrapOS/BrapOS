#include "Keyboard.hpp"

/// The US keyboard mapping between scancode and ASCII representation
const unsigned char KeyboardEntry::usMapping_[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`',   0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/',   0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/**
 * \brief Initialize the object
 *
 * \param scancode The scancode of the key that was used
 * \param isPressed true if the key was pressed
 * \param isLeftShiftPressed true if the left shift was pressed
 * \param isRightShiftPressed true if the right shift was pressed
 * \param isCtrlPressed true if the ctrl was pressed
 * \param isAltPressed true if the alt was pressed
 */
KeyboardEntry::KeyboardEntry(unsigned char scancode,  bool isPressed,
                             bool isLeftShiftPressed, bool isRightShiftPressed,
                             bool isCtrlPressed,      bool isAltPressed)
    : isPressed_(isPressed),
      isLeftShiftPressed_(isLeftShiftPressed),
      isRightShiftPressed_(isRightShiftPressed),
      isCtrlPressed_(isCtrlPressed), isAltPressed_(isAltPressed)
{
    // Convert the scancode to ASCII charcter
    character_ = usMapping_[scancode];
}

/**
 * \brief Get the character of the keyboard entry
 *
 * \return The character of the keyboard entry
 */
unsigned char KeyboardEntry::getCharacter() const
{
    return character_;
}

/**
 * \brief Return true if the keyboard entry was pressed
 *
 * \return true if the keyboard entry was pressed
 */
bool KeyboardEntry::isPressed() const
{
    return isPressed_;
}

/**
 * \brief Return true if the keyboard entry was used while the left shift was
 *        pressed
 *
 * \return true if the keyboard entry was used while the left shift was pressed
 */
bool KeyboardEntry::isLeftShiftPressed() const
{
    return isLeftShiftPressed_;
}

/**
 * \brief Return true if the keyboard entry was used while the right shift was
 *        pressed
 *
 * \return true if the keyboard entry was used while the right  shift was
 *         pressed
 */
bool KeyboardEntry::isRightShiftPressed() const
{
    return isRightShiftPressed_;
}

/**
 * \brief Return true if the keyboard entry was used while the ctrl was pressed
 *
 * \return true if the keyboard entry was used while the ctrl was pressed
 */
bool KeyboardEntry::isCtrlPressed() const
{
    return isCtrlPressed_;
}

/**
 * \brief Return true if the keyboard entry was used while the alt was pressed
 *
 * \return true if the keyboard entry was used while the alt was pressed
 */
bool KeyboardEntry::isAltPressed() const
{
    return isAltPressed_;
}

/// The `Keyboard` singleton instance
Keyboard Keyboard::instance_;

/**
 * \brief Initialize both indexes
 */
Keyboard::Keyboard()
    : readIndex_(0), writeIndex_(0)
{
}

/**
 * \brief Get the instance of the singleton object `Keyboard`
 *
 * \return the instance of the single oject of the class `Keyboard`
 */
Keyboard& Keyboard::getInstance()
{
    return instance_;
}

/**
 * \brief Get the `KeyboardEntry` pointed by the `readIndex_`
 *
 * This method returns the keyboard entry pointed by the reading index. It also
 * moves the reading index forward and makes it loop to the front of the buffer
 * if the end is reached.
 *
 * \return the `KeyboardEntry` pointed by the `readIndex_`
 */
KeyboardEntry Keyboard::readEntry()
{
    KeyboardEntry entry = buffer_[readIndex_];
    readIndex_ = (readIndex_ + 1) % CAPACITY;
    return entry;
}

/**
 * \brief Put a `KeyboardEntry` at the `writeIndex_`
 *
 * This method adds a keyboard entry in the buffer at the writing index. It
 * also moves the writing index forward and makes it loop to the front of the
 * buffer if the end is reached.
 *
 * \param entry the `KeyboardEntry` to add to the buffer
 */
void Keyboard::putEntry(const KeyboardEntry& entry)
{
    buffer_[writeIndex_] = entry;
    writeIndex_ = (writeIndex_ + 1) % CAPACITY;
}

/**
 * \brief Return true if the buffer is empty
 *
 * \return true if the buffer is empty
 */
bool Keyboard::isEmpty() const
{
    return readIndex_ == writeIndex_;
}
