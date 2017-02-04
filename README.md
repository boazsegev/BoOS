# Bo OS - a toy project for OS development

This is a toy project.

I rewrote the UEFI headers from the specification (UEFI Specs. v.2.6) so that this UEFI application wouldn't require any libraries to compile.

This project requires `mtoc` to finish compilation (converts the MachO executable to a Portable Executable format with the EFI required header). You can compile it as part of the developer tools from [OpenSource Apple](https://opensource.apple.com).

Testing is performed using [QEMU emulation](http://www.qemu.org) together with the [OVMF boot-loader](http://www.tianocore.org/ovmf/), which is part of the [tianocore](http://www.tianocore.org) EDK II package.

My goals:

* Create a simple OS that prints "BoOS booted" and nothing else.
* Get `clang` to run on BoOS.
* Slowly work my way towards and implement some core POSIX features.

You can use the [boilerplate version](https://github.com/boazsegev/BoOS/tree/v.0.0.0) if you want to start your own kernel and OS using macOS and `clang` for your development environment. However, the UEFI headers might have been updated since then, so you might want to copy these from the latest version.

Resources I read through or used:

* [OSDev website and data](http://wiki.osdev.org/)
* [EUFI specs](http://www.uefi.org/specsandtesttools)
* [A x64 OS #1: UEFI (Blog post for hello world kernel)](https://kazlauskas.me/entries/x64-uefi-os-1.html)
* [BIOS Interrupt table](https://en.wikipedia.org/wiki/BIOS_interrupt_call#Interrupt_table)
* [WikiBook X86 Assembly](https://en.wikibooks.org/wiki/X86_Assembly)
* [QEMU emulation](http://www.qemu.org) - I use this for loading and testing the kernel.
* [tianocore](http://www.tianocore.org) EDK II package - for the OVMF testing UEFI boot loader.
* [OpenSource Apple](https://opensource.apple.com) used to convert MachO to a UEFI compliant PE.
* There's an [MIT course](https://pdos.csail.mit.edu/6.828/2016/overview.html) with an [educational OS project](https://github.com/mit-pdos/xv6-public) I hadn't read yet.
