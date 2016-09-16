OBJECTS = src/boot.o src/kernel.o src/io.o src/Terminal.o src/vga/Screen.o
DEPS = $(OBJECTS:.o=.d)

AS = i686-elf-as
CXX = i686-elf-g++

CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
DEPFLAGS = -MT $@ -MMD -MP -MF $*.Td
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

POSTCOMPILE = mv -f $*.Td $*.d

KERNEL = brapos.bin

all: $(KERNEL)

$(KERNEL): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -T linker.ld -o $(KERNEL)

%.o: %.s
	$(AS) $< -o $@

%.o: %.cpp
%.o: %.cpp %.d
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

%.d: ;
.PRECIOUS: %.d

.PHONY: doc run clean

doc:
	doxygen Doxyfile

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

clean:
	rm -rf $(OBJECTS) $(DEPS) $(KERNEL) doc

-include $(DEPS)