KERNEL_NAME = B-Gyro
CC			= i386-elf-gcc
AS			= nasm
SRC_DIR 	= src
ARCH		= arch/i386
LDSCRIPT	= $(SRC_DIR)/$(ARCH)/linker.ld
TARGET		= $(KERNEL_NAME).bin
ISO			= $(KERNEL_NAME).iso
ISO_DIR		= build/isodir
COLOR_BLUE	= \033[96m
COLOR_WHITE	= \033[97m

# ANNOYING_FLAGS = -Wall -Wextra -Werror
ANNOYING_FLAGS = 

CFLAGS = -std=gnu99 -ffreestanding $(ANNOYING_FLAGS)\
		 -fno-builtin -nodefaultlibs -Isrc/include -mhard-float\
		 -mno-red-zone -fno-stack-protector -fno-omit-frame-pointer

ASFLAGS = -f elf32
LDFLAGS = -T $(LDSCRIPT) -ffreestanding -nostdlib

CSRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
CDATA = $(shell find $(SRC_DIR) -type f -name "*.cdata")
SSRCS = $(shell find $(SRC_DIR) -type f -name "*.s")

COBJECTS = $(patsubst $(SRC_DIR)/%,build/%,$(CSRCS:.c=.o))
CDATAOBJECTS = $(patsubst $(SRC_DIR)/%,build/%,$(CDATA:.cdata=.o))
SOBJECTS = $(patsubst $(SRC_DIR)/%,build/%,$(SSRCS:.s=.o))

OBJECTS  = ${COBJECTS} ${CDATAOBJECTS} ${SOBJECTS}

# Default target
all: header
	@docker compose run --rm build-env
#>/dev/null 2>/dev/null

dbg: re all
	bochs -q -dbg

# Rule to make the iso
dockerISO: $(TARGET)
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(TARGET) $(ISO_DIR)/boot/$(TARGET)
	@cp $(SRC_DIR)/$(ARCH)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISO) $(ISO_DIR)

# Rule to make the binary
$(TARGET): $(OBJECTS)
	@$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

run: all
	@unset GTK_PATH; qemu-system-i386 -m 2G -cdrom $(ISO) -k en-us\
	 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker\
	 -serial stdio -device pci-bridge,chassis_nr=1,id=pci.1 \
    -device e1000,netdev=net0,bus=pci.1,addr=0x2 \
    -netdev user,id=net0 2>/dev/null
	
# -no-reboot -no-shutdown

# Rule to make the object files
build/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

build/%.o: $(SRC_DIR)/%.cdata
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -x c -c $< -o $@

build/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@

# Clean rule
dockerClean:
	@rm -rf $(OBJECTS) $(TARGET) build/*

# re rule
re: fclean

# fclean rule
fclean:
	@docker compose run --rm clean-env
	@rm -rf $(ISO)

header:
	@echo "$(COLOR_BLUE)"
	@echo "███████████               █████████  █████ ██████ ██████████     ███████    "
	@echo "░░███░░░░░███             ███░░░░░███░░███ ░░███░ ░███░░░░░███  ███░░░░░███ "
	@echo " ░███    ░███            ███     ░░░  ░░███ ███   ░███    ░███ ███     ░░███"
	@echo " ░██████████  ██████████░███           ░░█████    ░██████████ ░███      ░███"
	@echo " ░███░░░░░███░░░░░░░░░░ ░███    █████   ░░███     ░███░░░░░███░███      ░███"
	@echo " ░███    ░███           ░░███  ░░███     ░███     ░███    ░███░░███     ███ "
	@echo " ███████████             ░░█████████     █████    █████   █████░░░███████░  "
	@echo "░░░░░░░░░░░               ░░░░░░░░░     ░░░░░    ░░░░░   ░░░░░   ░░░░░░░    $(COLOR_WHITE)"
	@echo "                                                           by IMANE and TAHA"
	@echo ""

.PHONY: all re fclean run dockerClean dockerISO header