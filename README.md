# S2Kernel
S2Kernel (Small and Simple Kernel) is a kernel for specific use cases.

### Requirements

* NASM
* GCC
* QEMU
* GRUB

> Note: If the kernel runs in WSL(Ubuntu) and use EFI for booting, run this command in order to build a successful build file `sudo apt install grub-common xorisso grub-pc-bin`.

### How to run

```bash
# 1. Build an ISO image
make build_x86_iso

# 2.a Run the ISO image
qemu-system-i386 -cdrom ./build/S2Kernel.iso

# 2.b Run the kernel via binary file
qemu-system-i386 -kernel ./build/S2Kernel.bin

# 3. Clean the project
make clean
```

### Features of S2Kernel
- [ ] Context Switching
- [ ] Interrupt
- [ ] Inter-process Communication (IPC)
- [ ] Memory Management
- [ ] Process Scheduling
