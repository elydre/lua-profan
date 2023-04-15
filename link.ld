/* Tell the linker that we want an ELF32 output file */
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

/* Linker script for an application for the OS. */

/* Entry point for application */
ENTRY (entry)

SECTIONS 
{
    . = 64M; /*Load at 64M, until we get paging working*/

    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}
