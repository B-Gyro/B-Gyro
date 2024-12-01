# OS design:
	- Modular kernel.
	- 32 bit.
	- written in C and assembly.
	- uses elf format for executables.
	- debugging helpers using serial communication.

# CodeBase Consistency:
	** FUNCTIONS:
		- functions and variables uses camelCase.
		- functions line count between 25 and 30.
		- functions can have up to 4 arguments.
	** VARIABLES:
		- Variables should not be assigned values at the time of declaration.
		- variables should be declared at the start of the function.
		- variables uses camelCase.
	** MACROS:
		- macros are in uppercase, with underscores.
	** STRUCTS:
		- structs always have typedef.
		- all typedes starts with underscore and uses camelCase.
	** UNIONS:
		- unions starts with u_;
	** GLOBALS:
		- unions starts with g_;
	** ENUMS:
		- unions starts with e_;
	** CODE FORMATING:
		- space tabulations.
		- Each function must be separated by a newline.
		- separate variable declaration and assingement by a new line.

# Source Tree consistency:
	- all includes should be under the include folder.
	- targeted code will reside under 'arch/<target-architecture>/'.
	- the targeted code's include will reside under 'include/arch/<the-component>.h'.
	 - Portability:
	- separate the targeted code from the generic code.
	   *Targeted:
			- things that have to do with the hardware, that should change in case you changed the architecture (from x86 to ARM).
			example of that would be writting to the video memory, paging and cpu specific functions.
		*Generic:
			- things that are pure software and shouldn't be effected by hardware change, example of that would be string parsing functions,
			or buffered I/O (gets, putchar...).