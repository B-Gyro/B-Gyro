#include "arch/i386/pci/pci.h"
#include "arch/i386/ports/portsIO.h"
#include "klibc/print.h"

void	checkBus(uint8_t bus);

char	*getDeviceBaseClass(uint8_t classCode){
	switch (classCode){
		case 0x00:
			return "Unclassified";
		case 0x01:
			return "Mass Storage Controller";
		case 0x02:
			return "Network Controller";
		case 0x03:
			return "Display Controller";
		case 0x04:
			return "Multimedia Controller";
		case 0x05:
			return "Memory Controller";
		case 0x06:
			return "Bridge";
		case 0x07:
			return "Simple Communication Controller";
		case 0x08:
			return "Base System Peripheral";
		case 0x09:
			return "Input Device Controller";
		case 0x0A:
			return "Docking Station";
		case 0x0B:
			return "Processor";
		case 0x0C:
			return "Serial Bus Controller";
		case 0x0D:
			return "Wireless Controller";
		case 0x0E:
			return "Intelligent Controller";
		case 0x0F:
			return "Satellite Communication Controller";
		case 0x10:
			return "Encryption Controller";
		case 0x11:
			return "Signal Processing Controller";
		case 0x12:
			return "Processing Accelerator";
		case 0x13:
			return "Non-Essential Instrumentation";
		case 0x40:
			return "CO-Processor";
		case 0xFF:
			return "Unassigned Class (Vendor specific)";
		default:
			if ((classCode >= 0x14 && classCode <= 0x3F) || (classCode >= 0x41 && classCode <= 0xFE))
				return "Reserved";
	}

	return NULL;
}


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

char	*get07Subclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "Serial Controller";
		case 0x01:
			return "Parallel Controller";
		case 0x02:
			return "Multiport Serial Controller";
		case 0x03:
			return "Modem";
		case 0x04:
			return "IEEE 488.1/2 (GPIB) Controller";
		case 0x05:
			return "Smart Card Controller";
		case 0x80:
			return "Other";
	}

	return NULL;
}

char	*get08Subclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "PIC";
		case 0x01:
			return "DMA Controller";
		case 0x02:
			return "Timer";
		case 0x03:
			return "RTC Controller";
		case 0x04:
			return "PCI Hot-Plug Controller";
		case 0x05:
			return "SD Host controller";
		case 0x06:
			return "IOMMU";
		case 0x80:
			return "Other";
	}

	return NULL;
}

char	*get09Subclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "Keyboard Controller";
		case 0x01:
			return "Digitizer Pen";
		case 0x02:
			return "Mouse Controller";
		case 0x03:
			return "Scanner Controller";
		case 0x04:
			return "Gameport Controller";
		case 0x80:
			return "other";
	}

	return NULL;
}

char	*get0ASubclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "Generic";
		case 0x80:
			return "Other";
	}
	return NULL;
}

char	*get0BSubclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "386";
		case 0x01:
			return "486";
		case 0x02:
			return "Pentium";
		case 0x10:
			return "Alpha";
		case 0x20:
			return "PowerPC";
		case 0x30:
			return "MIPS";
		case 0x40:
			return "CO-Processor";
		case 0x80:
			return "Other";
	}

	return NULL;
}

char	*get0CSubclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "FireWire (IEEE 1394) Controller ";
		case 0x01:
			return "ACCESS Bus Controller";
		case 0x02:
			return "SSA";
		case 0x03:
			return "USB Controller";
		case 0x04:
			return "Fibre Channel";
		case 0x05:
			return "SMBus Controller";
		case 0x06:
			return "InfiniBand Controller";
		case 0x07:
			return "IPMI Interface";
		case 0x08:
			return "SERCOS Interface (IEC 61491)";
		case 0x09:
			return "1CANbus Controller";
		case 0x80:
			return "Other";
	}

	return NULL;
}

char	*get0DSubclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "iRDA Compatible Controller";
		case 0x01:
			return "Consumer IR Controller";
		case 0x10:
			return "RF Controller";
		case 0x11:
			return "Bluetooth Controller";
		case 0x12:
			return "Broadband Controller";
		case 0x20:
			return "Ethernet Controller (802.1a)";
		case 0x21:
			return "Ethernet Controller (802.1b)";
		case 0x80:
			return "Other";
	}

	return NULL;
}

char	*get0ESubclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "I20";
		//case 0x80:
		//	return "Other";
	}

	return NULL;
}

char	*get0FSubclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x01:
			return "Satellite TV Controller";
		case 0x02:
			return "Satellite Audio Communication Controller";
		case 0x03:
			return "Satellite Voice Communication Controller";
		case 0x04:
			return "Satellite Data Communication Controller";
	}

	return NULL;
}

char	*get10Subclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "Network and Computing Encrpytion/Decryption";
		case 0x10:
			return "Entertainment Encryption/Decryption";
		case 0x80:
			return "Other";
	}

	return NULL;
}

char	*get11Subclass(uint8_t subclassCode){
	switch (subclassCode){
		case 0x00:
			return "DPIO Modules";
		case 0x01:
			return "Performance Counters";
		case 0x10:
			return "Communication Synchronizer";
		case 0x20:
			return "Signal Processing Management";
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
		case 0x07:
			return get07Subclass(subclassCode);
		case 0x08:
			return get08Subclass(subclassCode);
		case 0x09:
			return get09Subclass(subclassCode);
		case 0x0A:
			return get0ASubclass(subclassCode);
		case 0x0B:
			return get0BSubclass(subclassCode);
		case 0x0C:
			return get0CSubclass(subclassCode);
		case 0x0D:
			return get0DSubclass(subclassCode);
		case 0x0E:
			return get0ESubclass(subclassCode);
		case 0x0F:
			return get0FSubclass(subclassCode);
		case 0x10:
			return get10Subclass(subclassCode);
		default:
			return NULL;
	}
}

uint16_t	pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
	_pciConfigAddr	addr;
	uint32_t		tmp;

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

void	printDeviceInfo(_pciDeviceInfo device, uint8_t func){
	uint16_t deviceClass;

	if (!(device.functions[func]))
		return ;

	deviceClass = (device.classCode[func] << 8) | device.subclassCode[func];
	VGA_PRINT("[%d:%d:%d] V_ID:0x%04x D_ID:0x%04x C["COLOR_GREEN"%s"COLOR_DEFAULT"] SC["COLOR_CYAN"%s"COLOR_DEFAULT"]\n", device.bus, device.slot, func,\
		device.vendorID, device.deviceID, getDeviceBaseClass(device.classCode[func]), returnDeviceIdentifier(deviceClass));
	SERIAL_INFO("[%d:%d:%d] V_ID:0x%04x D_ID:0x%04x C["COLOR_GREEN"%s"COLOR_DEFAULT"] SC["COLOR_CYAN"%s"COLOR_DEFAULT"]", device.bus, device.slot, func,\
		device.vendorID, device.deviceID, getDeviceBaseClass(device.classCode[func]), returnDeviceIdentifier(deviceClass));
}

bool	fillDeviceInfo(_pciDeviceInfo *device, uint8_t func){
	uint8_t		bus, slot;
	uint16_t	deviceClass;

	bus = device->bus;
	slot = device->slot;
	if (pciConfigReadWord(bus, slot, func, VENDOR_ID_OFFSET) == 0xFFFF)
			return 0;
	device->functions[func] = TRUE;
	deviceClass = pciConfigReadWord(bus, slot, func, CLASS_CODE_OFFFSET);
	device->classCode[func] = H8(deviceClass);
	device->subclassCode[func] = L8(deviceClass);
	device->progIF = H8(pciConfigReadWord(bus, slot, func, PROG_IF_OFFSET));
	return 1;
}

void	checkDeviceFunctionalities(uint8_t bus, uint8_t slot){
	uint8_t			deviceFunc;
	_pciDeviceInfo	device = {0};

	deviceFunc = 0;
	device.vendorID = pciConfigReadWord(bus, slot, deviceFunc, VENDOR_ID_OFFSET);

	if (device.vendorID == 0xFFFF) // check if device doesn't Exist
		return ;

	device.bus = bus;
	device.slot = slot;
	device.deviceID = pciConfigReadWord(bus, slot, deviceFunc, DEVICE_ID_OFFSET);
	device.headerType = pciConfigReadWord(bus, slot, deviceFunc, HEADER_TYPE_OFFFSET);

	do{
		if (!fillDeviceInfo(&device, deviceFunc))
			continue;
		printDeviceInfo(device, deviceFunc);
		// check if the device is a pci-to-pci brige so we need to scan the next bus also;
		if ((device.classCode[deviceFunc] == 0x6) && (device.subclassCode[deviceFunc] == 0x4)){
			uint16_t primarySecondaryBus = pciConfigReadWord(bus, slot, deviceFunc, PRIMARY_SECONDARY_BUS_OFFSET);
			checkBus(H8(primarySecondaryBus));
		}
	} while ((device.headerType & (1 << 7)) && (++deviceFunc < 8));
}

void	checkBus(uint8_t bus){
	for (uint8_t slot = 0; slot < 32; slot++){
		checkDeviceFunctionalities(bus, slot);
	}
}

void	getPCIDevices(void){
	checkBus(0);
}
