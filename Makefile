CFLAGS = 	-ffreestanding -Wall -Wextra -fstack-protector \
			-fstack-protector-strong -fstack-protector-all \
			-fno-rtti -fno-exceptions -nostdlib -Ofast -lgcc \
			-Iinclude -pedantic

OBJS = build/boot.o build/libpaging.o build/libgdt.o build/libirq.o build/libsyscall.o

CXX = i686-elf-g++
CC = i686-elf-gcc
LD = i686-elf-gcc
AS = i686-elf-as

iso: kernel ensure_out_dir
	mkdir -p build/isodir/boot/grub
	cp build/kernel build/isodir/boot/kernel
	cp grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/must.iso build/isodir

run:
	qemu-system-x86_64 -drive file=build/must.iso,format=raw,index=0,media=disk

# link: asm kernel drivers ensure_out_dir
# 	$(LD) -T linker.ld -o build/hn_demo.bin $(OBJS) $(CFLAGS)

kernel: ensure_out_dir asm drivers
	$(CXX) -T linker.ld src/kernel.cpp $(OBJS) -o build/kernel $(CFLAGS)

asm: ensure_out_dir
	$(AS) src/boot.s -o build/boot.o
	$(AS) src/lib/paging.S -o build/libpaging.o
	$(AS) src/lib/gdt.S -o build/libgdt.o
	$(AS) src/lib/irq.S -o build/libirq.o
	$(AS) src/lib/syscall.S -o build/libsyscall.o

lib%.o: %.s
	$(AS) src/

drivers:

ensure_out_dir:
	mkdir -p build

clean:
	rm -rf build/
