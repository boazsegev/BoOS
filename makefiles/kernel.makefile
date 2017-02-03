KERNEL_ROOT:=kernel
#prepare Kernel Object List
KERNEL_SUBFOLDERS:= src
# public kernel include files
INCLUDE+= $(KERNEL_ROOT)/public/usr/include/
# Add kernel includes to the mix
RECURSIVE_COPY2SYSROOT+= $(KERNEL_ROOT)/public/
#EFI Boot folder
KERNEL_EFI_BOOT_FOLDER=EFI/BOOT

# The objects to compile
KERNEL_TREE:= $(KERNEL_ROOT)
KERNEL_TREE+= $(foreach dir, $(KERNEL_SUBFOLDERS), $(addsuffix /,$(basename $(KERNEL_ROOT)))$(dir))
KERNEL_SOURCES:= $(foreach dir, $(KERNEL_TREE), $(wildcard $(addsuffix /, $(basename $(dir)))*.c*))
KERNEL_OUTTREE:=$(foreach dir, $(KERNEL_TREE), $(addsuffix /, $(basename $(OUTROOT)))$(basename $(dir)))
KERNEL_OBJECTS:=$(foreach source, $(KERNEL_SOURCES), $(addprefix $(OUTROOT)/, $(addsuffix .o, $(basename $(source)))))
KERNEL_CFLAGS_COMMON= -target x86_64--macho \
											-ffreestanding -flto \
											-fno-exceptions -fno-rtti \
											-g0 -nostdlib \
											-std=c11
											# -iquote src/ \
											# -fno-builtin -fno-common -mkernel \
											# -mno-red-zone \
											# -nobuiltininc \
											# -fno-stack-protector \
											# -ffreestanding -flto \
											# -fno-exceptions -fno-rtti \
											# -g0 -nostdlib \
											# -target i686-pc-none-elf
											# -target x86_64-pc-win32-elf
											# -target x86_64-pc-mingw32-elf
											# -target x86_64--macho
KERNEL_LDFLAGS= -e _efi_main \
								-static \
								-fpie \
								-pagezero_size 0x0 \
								-image_base 0x1000 \
								-mmacosx-version-min=`sw_vers -productVersion` \
								-Wl,-macosx_version_min,`sw_vers -productVersion` \
								-arch x86_64

KERNEL_CFLAGS=  -emit-llvm \
								-nobuiltininc \
								-O \
								-fshort-wchar \
								-DHAVE_USE_MS_ABI

# the result
.PHONY : build_kernel
build_kernel: | .pre_build_kernel .compile_kernel
	-@echo "writing kernel to $(SYSROOT_OUT)/efi/boot/boot$(ARCH).efi"
	@./mtoc $(OUTROOT)/unstripped.efi $(SYSROOT_OUT)/$(KERNEL_EFI_BOOT_FOLDER)/BOOT$(ARCH).EFI


$(OUTROOT)/$(KERNEL_ROOT)/%.o : $(KERNEL_ROOT)/%.c
	@$(CC) $(CFLAGS) $(KERNEL_CFLAGS) $(KERNEL_CFLAGS_COMMON) -o $@ -c $^

.PHONY : .compile_kernel
.compile_kernel: $(KERNEL_OBJECTS)
	@$(CC) $(CFLAGS) $(KERNEL_CFLAGS_COMMON) $(KERNEL_LDFLAGS) -o $(OUTROOT)/unstripped.efi $^


.PHONY : .pre_build_kernel
.pre_build_kernel:
	-@echo "Starting to create the kernel."
	-@mkdir -p $(KERNEL_OUTTREE)
	-@mkdir -p $(SYSROOT_OUT)/$(KERNEL_EFI_BOOT_FOLDER)
