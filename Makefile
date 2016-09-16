OBJECTS = boot.o kernel.o
CXX = i686-elf-g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc
AS = i686-elf-as

KERNEL = brapos.bin

all: $(KERNEL)

$(KERNEL): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -T linker.ld -o $(KERNEL)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $< -o $@

.PHONY: doc run clean

doc:
	doxygen Doxyfile

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

clean:
	rm -rf $(OBJECTS) $(KERNEL)
	rm -rf doc
