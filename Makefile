TARGET = x86_64

CFLAGS = 	-ffreestanding -Wall -Wextra \
			-fno-rtti -fno-exceptions -nostdlib -Ofast -lgcc \
			-Isrc -pedantic \
			-D ARCH_$(TARGET) 

OBJS = build/boot.o

CXX = $(TARGET)-elf-g++
CC = $(TARGET)-elf-gcc
LD = $(TARGET)-elf-gcc
AS = $(TARGET)-elf-as

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
	$(CC) -c src/arch/boot.s -o build/boot.o $(CFLAGS)

ensure_out_dir:
	mkdir -p build

clean:
	rm -rf build/
