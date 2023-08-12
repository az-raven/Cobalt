TARGET ?= x86_64

CFLAGS = 	-ffreestanding -Wall -Wextra \
			-fno-rtti -fno-exceptions -nostdlib -O3 -lgcc \
			-Isrc -Isrc/include -pedantic --std=c++20 \
			-D ARCH_$(TARGET) -c -r -mcmodel=large -fPIE

ASFLAGS = 	-ffreestanding -Wall -Wextra \
			-fno-rtti -fno-exceptions -nostdlib -O3 -lgcc \
			-Isrc -Isrc/include -pedantic --std=c++20 \
			-D ARCH_$(TARGET) -c -r

LDFLAGS =	-m elf_$(TARGET) -T src/linker.ld -nostdlib \
			-static -fPIE -z text -z max-page-size=0x1000 \
			--no-dynamic-linker -shared

CXX = build/toolchain/bin/$(TARGET)-elf-g++
CC = build/toolchain/bin/$(TARGET)-elf-gcc
LD = build/toolchain/bin/$(TARGET)-elf-ld
AS = build/toolchain/bin/$(TARGET)-elf-gcc

LIMINE_DIR = build/toolchain/limine

TOOLCHAIN_GCC_VERSION = 13.2.0
TOOLCHAIN_BINUTILS_VERSION = 2.41

.PHONY: complete clean clean-kernel clean-toolchain clean-image toolchain kernel image install

complete: toolchain kernel image
	echo "Starting complete rebuild..."

run:
	qemu-system-x86_64 -drive format=raw,file=build/cobalt.img

bochs:
	bochs

install:
	# "WARNING: This automated LOCAL installation requires both a very specific system configuration and root access."
	# "Only run this if you know what you're doing!"
	# "Are you sure you want to continue? [y/N]"
	@read CONFIRM; \
	if [ $$CONFIRM = "y" ]; then \
		echo "Installing Cobalt locally..."; \
		sudo mkdir -p /boot/efi/EFI/Cobalt; \
		sudo cp -v build/kernel/cobalt src/limine.cfg $(LIMINE_DIR)/limine-bios.sys \
			/boot/efi/; \
		sudo cp -v $(LIMINE_DIR)/BOOTX64.EFI $(LIMINE_DIR)/BOOTIA32.EFI \
			/boot/efi/EFI/Cobalt/; \
		echo "Done!"; \
	else \
		echo "Aborting..."; \
	fi

kernel: build/kernel/init/main.o build/kernel/kernel.o build/kernel/include/string.o
	echo $@
	echo "Copying linker script..."
	cp src/linker.ld build/kernel/linker.ld
	echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o build/kernel/cobalt $^

build/kernel/kernel.o: src/kernel.cpp
	echo $@
	mkdir -p build/kernel
	$(CXX) -o $@ $< $(CFLAGS)

build/kernel/init/main.o: src/init/main.cpp
	echo $@
	mkdir -p build/kernel/init
	$(CXX) -o $@ $< $(CFLAGS)

build/kernel/include/string.o: src/include/string.cpp
	echo $@
	mkdir -p build/kernel/include
	$(CXX) -o $@ $< $(CFLAGS)

image:
	echo $@
	echo "Creating disk image..."
	dd if=/dev/zero bs=1M count=0 seek=64 of=build/cobalt.img

	echo "Creating partition table..."
	parted -s build/cobalt.img mklabel gpt

	echo "Creating ESP partition..."
	parted -s build/cobalt.img mkpart ESP fat32 2048s 100%
	parted -s build/cobalt.img set 1 esp on

	echo "Installing Limine boot sector..."
	$(LIMINE_DIR)/limine bios-install build/cobalt.img

	echo "Setting up loopback - this will require root."
	USED_LO=$$(sudo losetup -Pf --show build/cobalt.img); \
	\
	echo $$USED_LOAD; \
	\
	echo "Formatting ESP partition..."; \
	sudo mkfs.fat -F 32 $${USED_LO}p1; \
	\
	echo "Mounting ESP partition..."; \
	mkdir -p build/img_mount; \
	sudo mount $${USED_LO}p1 build/img_mount; \
	\
	echo "Setting up Limine EFI binaries..."; \
	sudo mkdir -p build/img_mount/EFI/BOOT; \
	sudo cp -v build/kernel/cobalt src/limine.cfg $(LIMINE_DIR)/limine-bios.sys \
		build/img_mount/; \
	sudo cp -v $(LIMINE_DIR)/BOOTX64.EFI $(LIMINE_DIR)/BOOTIA32.EFI \
		build/img_mount/EFI/BOOT/; \
	\
	echo "Cleaning up..."; \
	sync; \
	sudo umount build/img_mount; \
	sudo losetup -d $${USED_LO}

toolchain: build/toolchain/binutils-$(TOOLCHAIN_BINUTILS_VERSION) build/toolchain/gcc-$(TOOLCHAIN_GCC_VERSION) \
			build/toolchain/limine.h build/toolchain/limine
	echo $@
	echo "Building cross binutils..."
	mkdir -p build/toolchain/binutils_build

	cd build/toolchain/binutils_build && \
	../binutils-$(TOOLCHAIN_BINUTILS_VERSION)/configure \
		--target=$(TARGET)-elf \
		--prefix=$(shell pwd)/build/toolchain \
		--with-sysroot \
		--disable-nls \
		--disable-werror && \
	make -j12 && \
	make install

	echo "Building cross gcc..."
	mkdir -p build/toolchain/gcc_build

	cd build/toolchain/gcc_build && \
	../gcc-$(TOOLCHAIN_GCC_VERSION)/configure \
		--target=$(TARGET)-elf \
		--prefix=$(shell pwd)/build/toolchain \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers \
		--disable-werror && \
	make -j12 all-gcc && \
	make -j12 all-target-libgcc && \
	make install-gcc && \
	make install-target-libgcc

build/toolchain/binutils-$(TOOLCHAIN_BINUTILS_VERSION): build/toolchain/binutils-$(TOOLCHAIN_BINUTILS_VERSION).tar.gz
	echo $@
	echo "Extracting binutils source..."
	mkdir -p build/toolchain
	tar -zxvf build/toolchain/binutils-$(TOOLCHAIN_BINUTILS_VERSION).tar.gz -C build/toolchain

build/toolchain/binutils-$(TOOLCHAIN_BINUTILS_VERSION).tar.gz:
	echo $@
	echo "Downloading binutils source..."
	mkdir -p build/toolchain
	curl https://ftp.gnu.org/gnu/binutils/binutils-$(TOOLCHAIN_BINUTILS_VERSION).tar.gz > \
		build/toolchain/binutils-$(TOOLCHAIN_BINUTILS_VERSION).tar.gz

build/toolchain/gcc-$(TOOLCHAIN_GCC_VERSION): build/toolchain/gcc-$(TOOLCHAIN_GCC_VERSION).tar.gz
	echo $@
	echo "Extracting gcc source..."
	mkdir -p build/toolchain
	tar -zxvf build/toolchain/gcc-$(TOOLCHAIN_GCC_VERSION).tar.gz -C build/toolchain

build/toolchain/gcc-$(TOOLCHAIN_GCC_VERSION).tar.gz:
	echo $@
	echo "Downloading gcc source..."
	mkdir -p build/toolchain
	curl https://ftp.gnu.org/gnu/gcc/gcc-$(TOOLCHAIN_GCC_VERSION)/gcc-$(TOOLCHAIN_GCC_VERSION).tar.gz > \
		build/toolchain/gcc-$(TOOLCHAIN_GCC_VERSION).tar.gz

build/toolchain/limine:
	cd build/toolchain && git clone https://github.com/limine-bootloader/limine.git \
		--branch=v5.x-branch-binary --depth=1 && \
	make -C limine

build/toolchain/limine.h:
	echo $@
	echo "Downloading limine header file..."
	mkdir -p build/toolchain
	curl https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h > \
		build/toolchain/limine.h

clean:
	echo $@
	rm -rf build

clean-kernel:
	echo $@
	rm -rf build/kernel

clean-toolchain:
	echo $@
	rm -rf build/toolchain

clean-image:
	echo $@
	rm -rf build/img_mount build/cobalt.img
