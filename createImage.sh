#!/bin/bash
set -e

# Configuration
DISK_NAME="os-disk.img"
MOUNT_POINT="/mnt/disk"
KERNEL_FILE="B-Gyro.bin"

# Check kernel existence
if [ ! -f "$KERNEL_FILE" ]; then
    echo "Error: $KERNEL_FILE not found!"
    exit 1
fi

echo "Creating minimal disk image..."
dd if=/dev/zero of=$DISK_NAME bs=1M count=10

echo "Setting up partition table..."
parted -s $DISK_NAME mklabel msdos
parted -s $DISK_NAME mkpart primary ext2 1 10
parted -s $DISK_NAME set 1 boot on

echo "Setting up loop device..."
LOOP_DEVICE=$(sudo losetup --find --show $DISK_NAME)
echo "Using loop device: $LOOP_DEVICE"

echo "Updating partition table..."
sudo partprobe $LOOP_DEVICE
sleep 1

PART_DEVICE="${LOOP_DEVICE}p1"

echo "Formatting partition with minimal features..."
sudo mkfs.ext2 -b 1024 -O ^has_journal,^resize_inode -i 1024 $PART_DEVICE

echo "Mounting partition..."
sudo mkdir -p $MOUNT_POINT
sudo mount $PART_DEVICE $MOUNT_POINT

echo "Creating boot structure..."
sudo mkdir -p $MOUNT_POINT/boot/grub

echo "Copying kernel..."
sudo cp "$KERNEL_FILE" "$MOUNT_POINT/boot/"

echo "Installing minimal GRUB..."
# Install GRUB without extra modules and locales
sudo grub-install \
    --target=i386-pc \
    --boot-directory=$MOUNT_POINT/boot \
    --modules="biosdisk part_msdos ext2 multiboot" \
    --locales="" \
    --themes="" \
    --fonts="" \
    $LOOP_DEVICE

echo "Creating minimal GRUB config..."
sudo tee $MOUNT_POINT/boot/grub/grub.cfg << EOF
set timeout=0
set default=0

menuentry "B-Gyro OS" {
    multiboot /boot/B-Gyro.bin
}
EOF

echo "Cleaning up..."
sudo umount $MOUNT_POINT
sudo losetup -d $LOOP_DEVICE

echo "Disk image created successfully!"
echo "Run with: qemu-system-i386 -hda $DISK_NAME"