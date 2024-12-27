#include "arch/i386/pci/pci.h"
#include "arch/i386/ports/portsIO.h"
#include "klibc/print.h"

char	*g_deviceClasses[256] = {
	"Unclassified",
	"Mass Storage Controller",
	"Network Controller",
	"Display Controller",
	"Multimedia Controller",
	"Memory Controller",
	"Bridge",
	"Simple Communication Controller",
	"Base System Peripheral",
	"Input Device Controller",
	"Docking Station",
	"Processor",
	"Serial Bus Controller",
	"Wireless Controller",
	"Intelligent Controller",
	"Satellite Communication Controller",
	"Encryption Controller",
	"Signal Processing Controller",
	"Processing Accelerator",
	"Non-Essential Instrumentation",
};


char	*get00Subclass(uint8_t subclassCode){
	switch (subclassCode) {
		case 0x00:
			return "Non-VGA-Compatible Unclassified Device";		
		case 0x01:
			return "VGA-Compatible Unclassified Device";
	}
	return NULL;
}

char	*get01Subclass(uint8_t subclassCode){

	switch (subclassCode){
		case 0x00:
			return "SCSI Bus Controller";
		case 0x01:
			return "IDE Controller";
		case 0x02:
			return "Floppy Disk Controller";
		case 0x03:
			return "IPI Bus Controller";
		case 0x04:
			return "RAID Controller";
		case 0x05:
			return "ATA Controller"; 			
		case 0x06:
			return "Serial ATA Controller";
		case 0x07:
			return "Serial Attached SCSI Controller";
		case 0x08:
			return "Non-Volatile Memory Controller";
		case 0x80:
			return "Other";
	}
	return NULL;
}

char	*get02Subclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "Ethernet Controller";
		case 0x01:
			return "Token Ring Controller";
		case 0x02:
			return "FDDI Controller";
		case 0x03:
			return "ATM Controller";
		case 0x04:
			return "ISDN Controller";
		case 0x05:
			return "WorldFip Controller";
		case 0x06:
			return "PICMG 2.14 Multi Computing Controller";
		case 0x07:
			return "Infiniband Controller";
		case 0x08:
			return "Fabric Controller";
		case 0x80:
			return "Other";
	}
	return NULL;
}

char	*get03Subclass(uint8_t subclassCode){
	switch (subclassCode) {
		case 0x00:
			return "VGA Controller";
		case 0x01:
			return "XGA Controller";
		case 0x02:
			return "3D Controller";
		case 0x80:
			return "Other";
	}
	return NULL;
}

char	*get04Subclass(uint8_t subclassCode){
	switch (subclassCode) {
		case 0x00:
			return "Video Device";
		case 0x01:
			return "Audio Device";
		case 0x02:
			return "Computer Telephony Device";
		case 0x03:
			return "Audio Device";
		case 0x80:
			return "Other";
	}
	return NULL;
}

char	*get05Subclass(uint8_t subclassCode){
	switch (subclassCode) {
		case 0x00:
			return "RAM Controller";
		case 0x01:
			return "Flash Controller";
		case 0x80:
			return "Other";
	}
	return NULL;
}

char	*get06Subclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "Host Bridge";
		case 0x01:
			return "ISA Bridge";
		case 0x02:
			return "EISA Bridge";
		case 0x03:
			return "MCA Bridge";
		case 0x04:
			return "PCI-to-PCI Bridge";
		case 0x05:
			return "PCMCIA Bridge";
		case 0x06:
			return "NuBus Bridge";
		case 0x07:
			return "CardBus Bridge";
		case 0x08:
			return "RACEway Bridge";
		case 0x09:
			return "PCI-to-PCI Bridge";
		case 0x10:
			return "InfiniBand-to-PCI Host Bridge";
		case 0x80:
			return "Other";
	}
	return NULL;
}

char	*returnDeviceIdentifier(uint16_t class){
	uint8_t		classCode, subclassCode;


	classCode = class >> 8;
	subclassCode = class & 0xFF;
	switch (classCode) {
		case 0x00:
			return get00Subclass(subclassCode);
		case 0x01:
			return get01Subclass(subclassCode);
		case 0x02:
			return get02Subclass(subclassCode);
		case 0x03:
			return get03Subclass(subclassCode);
		case 0x04:
			return get04Subclass(subclassCode);
		case 0x05:
			return get05Subclass(subclassCode);
		case 0x06:
			return get06Subclass(subclassCode);
	default:
		return NULL;
	}
}

uint16_t	pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
	_pciConfigAddr	addr;
	uint32_t		tmp;

	/*
		Register Offset has to point to consecutive DWORDs.
		ie. bits 1:0 are always 0b00 (they are still part of the Register Offset).
	*/
	addr.registerOffset1 = (offset & 0xFC);
	addr.functionNumber = func;
	addr.deviceNumber = slot;
	addr.busNumber = bus;
	addr.reserved = 0;
	addr.enableBit = 1;

	portDWordOut(PCI_CONFIG_ADDRESS, *(uint32_t *)&addr);
	tmp = portDWordIn(0xCFC);
	// depending on the offset select the first of the last word
	tmp = (offset & 2) ? H16(tmp) : L16(tmp);

	return tmp;
}

void bruteForceGetPCIDevices(void) {
    uint8_t slot, func;
    uint16_t bus, vendorID, deviceID, deviceClass;

    for (bus = 0; bus < 256; bus++) {
        for (slot = 0; slot < 32; slot++) {

            for (func = 0; func < 8; func++) {
                // Read vendor ID
                vendorID = pciConfigReadWord(bus, slot, func, 0);
                if (vendorID != 0xFFFF) { // 0xFFFF means no device present
                    deviceID = pciConfigReadWord(bus, slot, func, 2);
					// read the class && subclass codes
					deviceClass = pciConfigReadWord(bus, slot, func, 0x0A);
					VGA_PRINT("%d:%d:%d VendorID:0x%x DeviceID:0x%x\n", bus, slot, func, vendorID, deviceID);
					VGA_PRINT("class: %s subclass: %s\n", g_deviceClasses[deviceClass >> 8], returnDeviceIdentifier(deviceClass));
					VGA_PRINT("--------------------\n");
                }
            }
        }
    }
}

