int	kmain(void){
	char *videoMemory = (char*)0xb8000;
	videoMemory[0] = '4';
	videoMemory[1] = 0x07;
	videoMemory[2] = '2';
	videoMemory[3] = 0x07;

	return 0;
}