# pragma once

# include "klibc/types.h"

/* MR:
	https://tldp.org/LDP/tlk/dd/pci.html
	https://wiki.osdev.org/PCI
*/

# define PCI_CONFIG_ADDRESS 0xCF8
# define PCI_CONFIG_DATA	0xCFC

# define DEVICE_ID_OFFSET		0x00
# define VENDOR_ID_OFFSET		0x02
# define CLASS_CODE_OFFFSET		0x0A
# define HEADER_TYPE_OFFFSET	0x0E

/*
Bit 31 		Bits 30-24 	Bits 23-16 	Bits 15-11 		Bits 10-8 			Bits 7-0
Enable Bit 	Reserved 	Bus Number 	Device Number 	Function Number 	Register Offset1
*/

typedef struct pciConfigAddr {	
    uint8_t registerOffset1:8;
    uint8_t functionNumber:3;
    uint8_t deviceNumber:5; // also the slot number
    uint8_t busNumber:8;
    uint8_t reserved:7;
    uint8_t enableBit:1;
} __attribute__((packed)) _pciConfigAddr;

typedef struct pciDeviceInfo{
	uint8_t		bus;
	uint8_t		slot;
	bool		functions[8];
	uint16_t	deviceID;
	uint16_t	vendorID;
	uint8_t		headerType;
	uint8_t		progIF;
	uint8_t		classCode[8];
	uint8_t		subclassCode[8];
}_pciDeviceInfo;


void	getPCIDevices(void);