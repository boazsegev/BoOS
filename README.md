# Bo OS - a toy project for OS development

This is a toy project.

I rewrote the UEFI headers from the specification (UEFI Specs. v.2.6) so that this UEFI application wouldn't require any libraries to compile.

This project requires `mtoc` to finish compilation (converts the MachO executable to a Portable Executable format with the EFI required header). You can compile it as part of the developer tools from [OpenSource Apple](https://opensource.apple.com).

Testing is performed using [OVMF](http://www.tianocore.org/ovmf/), which is part of the [tianocore](http://www.tianocore.org) EDK II package.

My goals:

* Create a simple OS that prints "BoOS booted" and nothing else.
* Get `clang` to run on BoOS.
* Slowly work my way towards and implement some core POSIX features.

To recycle this work (just use UEFI headers and boot a hello world type application), keep just the `makefiles` folder, the `makefile` file and the `kernel` folder. From the `kernel` folder you can delete everything except the UEFI headers and write your own code into `start.c`.

Resources I read through or used:

* [OSDev website and data](http://wiki.osdev.org/)
* [EUFI specs](http://www.uefi.org/specsandtesttools)
* [A x64 OS #1: UEFI (Blog post for hello world kernel)](https://kazlauskas.me/entries/x64-uefi-os-1.html)
* [BIOS Interrupt table](https://en.wikipedia.org/wiki/BIOS_interrupt_call#Interrupt_table)
* [WikiBook X86 Assembly](https://en.wikibooks.org/wiki/X86_Assembly)
* [tianocore](http://www.tianocore.org) EDK II package - for the OVMF testing UEFI boot loader.
* [OpenSource Apple](https://opensource.apple.com) used to convert MachO to a UEFI compliant PE.
