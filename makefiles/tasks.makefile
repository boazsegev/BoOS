CFLAGS+= $(WARNINGS)
CFLAGS+= --sysroot=$(SYSROOT_OUT)
CFLAGS+= $(foreach dir,$(INCLUDE),$(addprefix -I, $(dir)))
CFLAGS+= $(foreach def,$(DEFINE),$(addprefix -D, $(def)))

.PHONY : perform_copy
perform_copy:
	-@mkdir -p $(SYSROOT_OUT)
	-@cp -R -n $(RECURSIVE_COPY2SYSROOT) $(SYSROOT_OUT)

.PHONY : build
build: | perform_copy build_kernel create_iso

.PHONY : clean
clean:
	-@rm -R $(OUTROOT)

.PHONY : run
run: | default
	@qemu-system-x86_64 -drive if=pflash,file=OVMF.fd,format=raw -drive file=$(OUTROOT)/boos.iso,format=raw

.PHONY : debug
debug: | default
	@qemu-system-x86_64 -drive if=pflash,file=OVMF.fd,format=raw -debugcon file:ovmf_debug.log \
											-global isa-debugcon.iobase=0x402 -d guest_errors,cpu_reset,in_asm -S -s -no-reboot \
											-drive file=$(OUTROOT)/boos.iso,format=raw

# @qemu-system-x86_64 -smbios type=0,uefi=on -drive file=$(OUTROOT)/boos.iso,format=raw
# @qemu-system-x86_64 -pflash OVMF.fd -debugcon file:ovmf_debug.log -global isa-debugcon.iobase=0x402 -d guest_errors,cpu_reset,in_asm -S -s -no-reboot ./$(OUTROOT)/boos.iso
