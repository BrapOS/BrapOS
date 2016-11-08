CXX = i686-elf-g++
AS  = i686-elf-as

CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
DEPFLAGS = -MT $@ -MMD -MP -MF $*.Td
LDFLAGS  = -ffreestanding -O2 -nostdlib -lgcc

OBJECTS     = src/boot.o src/kernel.o src/util/util.o src/io.o src/Terminal.o src/vga/Screen.o src/SerialPort.o
CRTI_OBJECT = crti.o
CRTBEGIN_OBJECT := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJECT   := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTN_OBJECT = crtn.o
ALL_OBJECTS := $(CRTI_OBJECT) $(CRTBEGIN_OBJECT) $(OBJECTS) $(CRTEND_OBJECT) $(CRTN_OBJECT)
DEPS = $(OBJECTS:.o=.d)

POSTCOMPILE = mv -f $*.Td $*.d

KERNEL = brapos.bin

all: $(KERNEL)

$(KERNEL): $(ALL_OBJECTS)
	$(CXX) $(LDFLAGS) $(ALL_OBJECTS) -T linker.ld -o $(KERNEL)

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
	qemu-system-i386 -kernel $(KERNEL) -serial stdio

clean:
	rm -rf $(OBJECTS) $(DEPS) $(KERNEL) doc

-include $(DEPS)
