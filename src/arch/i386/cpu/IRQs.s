[extern irqHandler]

; common IQR handler:
irq_common:
	pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call irqHandler

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	popa
	add esp, 8
	sti
	iret

%macro IRQ 2
	[GLOBAL irq%1]
	irq%1:
	cli
	push byte %1
	push byte %2
	jmp irq_common
%endmacro

; defining all the IRQs
IRQ		0,		32	; 0:  System timer
IRQ		1,		33	; 1:  Keyboard Controller
IRQ		2,		34	; 2:  Receives signals from IRQs 8-15 (cascading)
IRQ		3,		35	; 3:  ??
IRQ		4,		36	; 4:  ??
IRQ		5,		37	; 5:  ??
IRQ		6,		38	; 6:  ??
IRQ		7,		39	; 7:  ??
IRQ		8,		40	; 8:  ??
IRQ		9,		41	; 9:  ??
IRQ		10,		42	; 10: ??
IRQ		11,		43	; 11: ??
IRQ		12,		44	; 12: ??
IRQ		13,		45	; 13: ??
IRQ		14,		46	; 14: ??
IRQ		15,		47	; 15: ??