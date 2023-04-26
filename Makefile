CFLAGS = 	-ffreestanding -Wall -Wextra \
			-fno-rtti -fno-exceptions -nostdlib -Ofast -lgcc \
			-Isrc -pedantic

OBJS = build/boot.o

CXX = x86_64-elf-g++
CC = x86_64-elf-gcc
LD = x86_64-elf-gcc
AS = x86_64-elf-as

iso: kernel ensure_out_dir
	mkdir -p build/isodir/boot/grub
	cp build/kernel build/isodir/boot/kernel
	cp grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/cobalt.iso build/isodir

run:
	qemu-system-x86_64 -drive file=build/cobalt.iso,format=raw,index=0,media=disk

kernel: ensure_out_dir asm
	$(CXX) -T linker.ld src/kernel.cpp $(OBJS) -o build/kernel $(CFLAGS)

asm: ensure_out_dir
	$(AS) src/arch/boot.s -o build/boot.o

ensure_out_dir:
	mkdir -p build

clean:
	rm -rf build/
