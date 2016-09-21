# BrapOS

A minimalist C++ operating system to learn OS development basics

Currently, BrapOS supports:

* Writing to the screen
* Sending data over the serial port

## Compilation

You need a GCC cross-compiler that targets the i686-elf architecture as detailled on [OSDev](http://wiki.osdev.org/GCC_Cross-Compiler).

To compile the source code and build the kernel image, run `make`.

To optionally generate the documentation with Doxygen, run `make doc`.

## Execution

One way to try BrapOS is to use the [QEMU emulator](http://wiki.qemu.org/Main_Page). You can start QEMU for BrapOS with `make run`.

## License

BrapOS is released under the [MIT License](LICENSE).
