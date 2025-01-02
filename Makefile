KERNEL_NAME = B-Gyro
CC			= i386-elf-gcc
AS			= nasm
SRC_DIR 	= src
ARCH		= arch/i386
LDSCRIPT	= $(SRC_DIR)/$(ARCH)/linker.ld
TARGET		= $(KERNEL_NAME).bin
ISO			= $(KERNEL_NAME).iso
ISO_DIR		= build/isodir

SERIAL_DEBUG = 1

CFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -Werror\
		 -fno-builtin -nodefaultlibs -Isrc/include\
		 -mno-red-zone -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2\
		 -fno-stack-protector -fno-omit-frame-pointer

ifeq ($(SERIAL_DEBUG), 1)
	CFLAGS += -DDEBUG
endif

ASFLAGS = -f elf32
LDFLAGS = -T $(LDSCRIPT) -ffreestanding -nostdlib -lgcc

CSRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
SSRCS = $(shell find $(SRC_DIR) -type f -name "*.s")

COBJECTS = $(patsubst $(SRC_DIR)/%,build/%,$(CSRCS:.c=.o))
SOBJECTS = $(patsubst $(SRC_DIR)/%,build/%,$(SSRCS:.s=.o))
OBJECTS  = ${COBJECTS} ${SOBJECTS}

# Default target
all:
	docker-compose run --rm build-env

# Rule to make the iso
dockerISO: $(TARGET)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(TARGET) $(ISO_DIR)/boot/$(TARGET)
	cp $(SRC_DIR)/$(ARCH)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Rule to make the binary
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

run: all
	unset GTK_PATH; qemu-system-i386 -cdrom $(ISO) -k en-us\
	 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker\
	 -serial stdio\
	 -device pci-bridge,chassis_nr=1,id=pci.1 \
	 -device e1000,bus=pci.1,addr=0x02 \
	 -device ne2k_pci,bus=pci.1,addr=0x03
# -no-reboot -no-shutdown

# Rule to make the object files
build/%.o: $(SRC_DIR)/%.c
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) -c $< -o $@

build/%.o: $(SRC_DIR)/%.s
			mkdir -p $(dir $@)
			$(AS) $(ASFLAGS) $< -o $@

# Clean rule
dockerClean:
	rm -rf $(OBJECTS) $(TARGET) build/*

# re rule
re: fclean all

# fclean rule
fclean:
	docker-compose run --rm clean-env
	rm -rf $(ISO)

.PHONY: all re fclean run dockerClean dockerISO
