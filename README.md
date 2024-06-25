# S2Kernel
S2Kernel (Small and Simple Kernel) is a kernel for specific use cases.

### Requirements

* NASM
* GCC
* QEMU

> Note: If the kernel runs in WSL(Ubuntu) and use EFI for booting, run this command to build a successful build file `sudo apt install grub-common xorisso grub-pc-bin`

### How to run

```bash
make
qemu-system-i386 ./build/S2Kernel.iso
```
