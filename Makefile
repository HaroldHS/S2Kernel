# Programs
ASM=nasm
GCC=gcc
LINKER=ld
GRUB=grub-mkrescue

# Flags for programs
CFLAGS_32_BITS= -m32 -fno-builtin

BUILD_DIR=build
KERNEL_DIR=kernel
SRC_DIR = src/x86
RESULT_BOOT_DIR=result/boot

# List of object files
ASSEMBLY_SOURCE_FILES=$(shell find $(SRC_DIR) -name *.asm)
ASSEMBLY_OBJECT_FILES=$(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASSEMBLY_SOURCE_FILES))
KERNEL_SOURCE_FILES=$(shell find $(KERNEL_DIR) -name *.c)
KERNEL_OBJECT_FILES=$(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_SOURCE_FILES))

.PHONY: build_x86_iso clean

build_x86_iso : $(BUILD_DIR)/S2Kernel.iso
# Generate ISO image
$(BUILD_DIR)/S2Kernel.iso : $(SRC_DIR)/$(RESULT_BOOT_DIR)/S2Kernel.bin
	$(GRUB) -o $(BUILD_DIR)/S2Kernel.iso $(SRC_DIR)/result
# Copy build result to /boot
$(SRC_DIR)/$(RESULT_BOOT_DIR)/S2Kernel.bin : $(BUILD_DIR)/S2Kernel.bin
	cp $(BUILD_DIR)/S2Kernel.bin $(SRC_DIR)/$(RESULT_BOOT_DIR)/S2Kernel.bin
# Generate binary image with linker
$(BUILD_DIR)/S2Kernel.bin : $(SRC_DIR)/linker.ld $(ASSEMBLY_OBJECT_FILES) $(KERNEL_OBJECT_FILES)
	$(LINKER) -m elf_i386 -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/S2Kernel.bin $(ASSEMBLY_OBJECT_FILES) $(KERNEL_OBJECT_FILES)
# Generate object files
$(ASSEMBLY_OBJECT_FILES) : $(SRC_DIR)/%.o : $(BUILD_DIR)/%.asm
	mkdir -p $(dir $@) && $(ASM) -f elf32 $(patsubst $(BUILD_DIR)/%.o, $(SRC_DIR)/%.asm, $@) -o $@
$(KERNEL_OBJECT_FILES): $(BUILD_DIR)/%.o : $(KERNEL_DIR)/%.c
	mkdir -p $(dir $@) && $(GCC) $(CFLAGS_32_BITS) -c $(patsubst $(BUILD_DIR)/%.o, $(KERNEL_DIR)/%.c, $@) -o $@

clean:
	rm -rf ./build/*
	rm ./src/x86/result/boot/S2Kernel.bin
