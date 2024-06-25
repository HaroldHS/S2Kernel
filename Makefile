ASM=nasm
GCC=gcc
LINKER=ld
GRUB=grub-mkrescue

BUILD_DIR=build
KERNEL_DIR=kernel
SRC_DIR = src/x86
RESULT_BOOT_DIR=result/boot

# Generate ISO image
$(BUILD_DIR)/S2Kernel.iso : $(SRC_DIR)/$(RESULT_BOOT_DIR)/S2Kernel.bin
	$(GRUB) -o $(BUILD_DIR)/S2Kernel.iso $(SRC_DIR)/result
# Copy build result to /boot
$(SRC_DIR)/$(RESULT_BOOT_DIR)/S2Kernel.bin : $(BUILD_DIR)/S2Kernel.bin
	cp $(BUILD_DIR)/S2Kernel.bin $(SRC_DIR)/$(RESULT_BOOT_DIR)/S2Kernel.bin
# Generate binary image with linker
$(BUILD_DIR)/S2Kernel.bin : $(SRC_DIR)/linker.ld $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
	$(LINKER) -m elf_i386 -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/S2Kernel.bin $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
# Generate object files
$(BUILD_DIR)/boot.o : $(SRC_DIR)/boot.asm
	$(ASM) -f elf32 $(SRC_DIR)/boot.asm -o $(BUILD_DIR)/boot.o
$(BUILD_DIR)/kernel.o : $(KERNEL_DIR)/kernel.c
	$(GCC) -m32 -c $(KERNEL_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o
