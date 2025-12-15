# Kernel From Scratch [B-Gyro]
Hey there! Welcome to our **Kernel From Scratch (KFS)** project. This started out as part of the **42 Network** curriculum, where we’re building an OS kernel from scratch. But here’s the fun part: we’re not just sticking to the basics. We’re adding our own features, customizing it, and going above and beyond the requirements.

Think of it as a fun, experimental project where we’re diving deeper into the world of systems programming and learning along the way. We’re taking the opportunity to add our own spin, explore new ideas, and build something that’s truly ours.

## To do:
### Base
- [x] An ASM bootable base that handles multiboot header
- [x] A linker
- [x] A basic kernel library, with basics functions and types

---
- [x] GDT
- [x] IDT
- [x] PCI
- [x] PIT
- [x] RTC
  - [x] Date
  - [x] time
    - [x] 24h mode
    - [x] 12h mode
- [x] Keyboard
  - [x] Querty
  - [x] Azerty

---
### Little Tiny Shell
- [x] Minimalistic shell
    - [x] clear
    - [x] reboot
    - [x] shutdown
    - [x] logout
    - [x] peek
    - [x] poke
    - [x] lspci
    - [x] screentime
    - [x] datetime
    - [x] timer
    - [x] history
    - [ ] help
- [x] Scroll 
- [x] Cursor 
- [x] History navigation
- [x] Different Screens (3 TTYs for now)

---

### Video Memory
- [x] VGA
    - [x] Text Mode (16 color)
      - [x] 80x25
      - [x] 80x50
    - [x] Video Mode
      - [x] 320x200x256
      - [x] 640x480x16
- [x] Fonts
  - [x] 8x8
  - [x] 8x16
  - [x] 9x14
  - [x] 9x16
- [x] Print
    - [x] Serial print
    - [x] VGA print
---

### Memory
- [ ] Enable paging
- [ ] User Space Memory
- [ ] Physical Memory
- [ ] Virtual Memory
- [ ] Helpers for physical memory
    - [ ] kmalloc
    - [ ] kfree
    - [ ] ksize
    - [ ] kbrk 
- [ ] Hhelpers for virtual memory
    - [ ] vmalloc
    - [ ] vfree
    - [ ] vsize
    - [ ] vbrk 

---

### Some Graphics
- [x] Draw Line
- [x] Draw Shapes (Empty + Filled)
  - [x] Rectangle + Square
  - [x] Triangle
  - [x] Circle
  - [x] Rhombus
- [x] Draw Array of pixels (To draw images)
