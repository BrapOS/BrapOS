CXX = i686-elf-g++
AS  = i686-elf-as

CXXFLAGS = -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti -g
DEPFLAGS = -MT $@ -MMD -MP -MF $*.Td
LDFLAGS  = -ffreestanding -nostdlib -lgcc

OBJECTS     = src/boot.o src/kernel.o src/util/util.o src/io.o src/Terminal.o src/vga/Screen.o src/SerialPort.o src/interrupt.o src/Keyboard.o src/KernelLogger.o
CRTI_OBJECT = src/crti.o
CRTBEGIN_OBJECT := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJECT   := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTN_OBJECT = src/crtn.o
ALL_OBJECTS := $(CRTI_OBJECT) $(CRTBEGIN_OBJECT) $(OBJECTS) $(CRTEND_OBJECT) $(CRTN_OBJECT)
DEPS = $(OBJECTS:.o=.d)

POSTCOMPILE = mv -f $*.Td $*.d

KERNEL = brapos.bin
KERNEL_ISO = brapos.iso
ISODIR = isodir
GRUB_CONFIG = grub.cfg

all: $(KERNEL)

$(KERNEL): $(ALL_OBJECTS)
	$(CXX) $(LDFLAGS) $(ALL_OBJECTS) -T linker.ld -o $(KERNEL)

$(KERNEL_ISO): $(KERNEL)
	mkdir -p $(ISODIR)/boot/grub
	cp $(KERNEL) $(ISODIR)/boot/$(KERNEL)
	cp $(GRUB_CONFIG) $(ISODIR)/boot/grub/$(GRUB_CONFIG)
	grub-mkrescue -o $(KERNEL_ISO) $(ISODIR)

%.o: %.s
	$(AS) $< -o $@

%.o: %.cpp
%.o: %.cpp %.d
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

%.d: ;
.PRECIOUS: %.d

.PHONY: doc iso gdb qemu bochs gdb clean

doc:
	doxygen Doxyfile

iso: $(KERNEL_ISO)

qemu: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL) -serial stdio -s

bochs: $(KERNEL_ISO)
	bochs -f bochsrc.txt -q

gdb: $(KERNEL)
	gdb -x init.gdb

clean:
	rm -rf $(OBJECTS) $(CRTI_OBJECT) $(CRTN_OBJECT) $(DEPS) $(KERNEL) $(KERNEL_ISO) $(ISODIR) doc

-include $(DEPS)
