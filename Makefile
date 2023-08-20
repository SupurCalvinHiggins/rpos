CFILES = $(wildcard *.c)
SFILES = $(wildcard *.S)

OCFILES = $(CFILES:.c=.o)
OSFILES = $(SFILES:.S=.o)

GCCFLAGS = -Wall -O0 -ffreestanding
GCCPATH = arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin

all: clean kernel8.img

%.o: %.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: $(OSFILES) $(OCFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib $(OSFILES) $(OCFILES) -T link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true
