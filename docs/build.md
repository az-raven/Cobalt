# Building Cobalt
---
> NOTE: This build guide covers a relatively wide range of
> operating systems and configurations, but Cobalt does not
> officially support any architectural and hardware configurations
> beyond those I (raven) use myself. 
>
> Hardware compatibility is **NOT** a priority for Cobalt,
> the priority is reliability, security, performance, and
> general usability. If you wish to support a specific device
> or configuration, **you** are expected to implement and
> maintain it yourself.
> 
> With that out of the way, have fun!

# Installing Dependencies
---

> NOTE: This assumes you have basic things like GCC, curl, make, etc installed.
> Other distributions are not officially supported as they're not what I 
> personally use and understand. If you have a thorough understanding of your
> distro, feel free to add onto this guide.

## Arch
```sh
sudo pacman -S base-devel gmp libmpc mpfr
```

# Quickstart
---
This is the minimum amount of effort required to get a running Cobalt instance.
If you do this more than once, it's super inefficient. I strongly recommend reading
futher if you intend to use Cobalt again.

```sh
make clean complete run
```

This will:
- Remove any existing build artifacts
- Build the toolchain
- Build the kernel
- Build the disk image
- Run the disk image in a virtual machine

# Complete Build (default, requires root)
---
A complete build of Cobalt will build the toolchain, kernel, and set up a disk image
with basic utilities. This is not necessary after the first build, as you will
already have a full toolchain available after that.

```sh
make complete
```

This is equivalent to

```sh
make toolchain kernel image
```

This will build the toolchain (located in `build/toolchain/**/*`) and the kernel,
before creating an empty disk image (`build/disk`), and attempting to mount it at
`build/root`. You will be prompted for your password, as this uses `sudo mount`.

# Rebuild Toolchain
---
This will rebuild the toolchain, but not the kernel or disk image. This is useful
after a compiler version update.

```sh
make toolchain
```

# Rebuild Kernel
---
This does not install the new kernel into the disk image or your local sysem,
but rebuilds it.

```sh
make kernel
```

# Rebuild Disk Image
---

```sh
make image
```

# Running Cobalt (Virtual Machine)
---
You must have `qemu-system-TARGET` installed for this to function, as well as a
fully built image.

```
make run
```

# Running Cobalt (Bare Metal)
---
> WARNING: This is designed for an extremely specific system configuration, and
> will require further modification even if it works. I assume no responsibility
> for anything you mess up using this.

## Installing to ESP (EFI System Partition)
This assumes that your ESP is mounted at `/boot/efi`, and that you have root
access on your machine.

```sh
make install
```

This does not automatically create a UEFI boot entry, you will need to do that
yourself. You can find information about this process in the documentation for
`efibootmgr`.

## UEFI Resources
- [UEFI - Arch Wiki](https://wiki.archlinux.org/index.php/Unified_Extensible_Firmware_Interface)
- [efibootmgr - Gentoo Wiki](https://wiki.gentoo.org/wiki/Efibootmgr)
- [efibootmgr - Arch Wiki](https://wiki.archlinux.org/index.php/EFISTUB#efibootmgr)

# Cleaning Up
---
## Complete
This will remove all build artifacts, including the toolchain, kernel, and disk
image.

```sh
make clean
```

## Toolchain Only
This will remove the toolchain, but not the kernel or disk image.

```sh
make clean-toolchain
```

## Kernel Only
This will remove the kernel, but not the toolchain or disk image.

```sh
make clean-kernel
```

## Disk Image Only
This will remove the disk image, but not the toolchain or kernel.

```sh
make clean-image
```
