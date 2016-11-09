#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * \brief Encapsulate the information related to a keyboard entry
 *
 * This object contains the informations regarding a keyboard entry.
 */
class KeyboardEntry
{
public:
    KeyboardEntry() {};
    /// Initialize the object
    KeyboardEntry(unsigned char scancode,  bool isPressed,
                  bool isLeftShiftPressed, bool isRightShiftPressed,
                  bool isCtrlPressed,      bool isAltPressed);

    /// Get the character of the keyboard entry
    unsigned char getCharacter() const;
    /// Return true if the keyboard entry was pressed
    bool isPressed() const;
    /// Return true if the keyboard entry was used while the left shift was
    /// pressed
    bool isLeftShiftPressed() const;
    /// Return true if the keyboard entry was used while the right shift was
    /// pressed
    bool isRightShiftPressed() const;
    /// Return true if the keyboard entry was used while the ctrl was pressed
    bool isCtrlPressed() const;
    /// Return true if the keyboard entry was used while the alt was pressed
    bool isAltPressed() const;

private:
    /// The character
    unsigned char character_;
    /// Boolean representing if the keyboard entry was pressed
    bool isPressed_;
    /// Boolean representing if the left shift was pressed
    bool isLeftShiftPressed_;
    /// Boolean representing if the right shift was pressed
    bool isRightShiftPressed_;
    /// Boolean representing if the ctrl was pressed
    bool isCtrlPressed_;
    /// Boolean representing if the alt was pressed
    bool isAltPressed_;

    /// The US keyboard mapping between scancode and ASCII representation
    static const unsigned char usMapping_[128];
};

/**
 * \brief Contains the the keyboard entry recently used
 *
 * This singleton object saves all the keys that were pressed and released in a
 * `KeyboardEntry` buffer. The buffer has a fixed size and is looping on itself
 * once the end is reached. There are two indexes, one indicating where to
 * start reading from and the other one representing where to add the new
 * keyboard entries.
 */
class Keyboard
{
public:
    /// Get the instance of the singleton object `Keyboard`
    static Keyboard& getInstance();

    /// Get the `KeyboardEntry` pointed by the `readIndex_`
    KeyboardEntry readEntry();
    /// Put a `KeyboardEntry` at the `writeIndex_`
    void putEntry(const KeyboardEntry& entry);
    /// Return true if the buffer is empty
    bool isEmpty() const;

    /// The copy constructor and copy assignment operator are deleted
    /// since the is a singleton
    Keyboard(Keyboard const&) = delete;
    void operator=(Keyboard const&) = delete;

private:
    /// Initialize both indexes
    Keyboard();

    /// The `Keyboard` singleton instance
    static Keyboard instance_;

    /// The read index
    uint32_t readIndex_;
    /// The write index
    uint32_t writeIndex_;
    /// The capacity of the buffer
    static const uint32_t CAPACITY = 1024;
    /// The `KeyboardEntry` buffer
    KeyboardEntry buffer_[CAPACITY];
};
