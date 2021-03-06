/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/
#include "defines.h"

#include "isr.h"
#include "idt.h"
#include "i386.h"



idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
	sizeof(idt) - 1,
	(unsigned int) &idt
}; 	


/*
	La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
	interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
	atributos y el registro de segmento. Invocarla desde idt_inicializar() de
	la siguiene manera:

	void idt_inicializar() {
		IDT_ENTRY(0);
		...
		IDT_ENTRY(19);

		...
	}
*/


#define IDT_ENTRY(numero, seg, atrr)                                                                            \
	idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
	idt[numero].segsel = (unsigned short) seg;                                                                  \
	idt[numero].attr = (unsigned short) atrr; 	                                                                 \
	idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

// Segmento 8 en la GDT: 1 * 2^3 (3 shifts) = 64
// Atributos:
// |   | D   |           |       |           | 
// | P | P   | 0 D 1 1 0 | 0 0 0 | 0 0 0 0 0 |
// |   | L   |           |       |           |

// Kernel
// | 1 | 0 0 | 0 1 1 1 0 | 0 0 0 | 0 0 0 0 0 | = 0x8E00

// Arbitro
// | 1 | 1 0 | 0 1 1 1 0 | 0 0 0 | 0 0 0 0 0 | = 0xCE00

// Jugador
// | 1 | 1 1 | 0 1 1 1 0 | 0 0 0 | 0 0 0 0 0 | = 0xEE00

#define GATE_TYPE_0 0x8E00
#define GATE_TYPE_2 0xCE00
#define GATE_TYPE_3 0xEE00

void idt_inicializar() {
	IDT_ENTRY(0,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(1,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(2,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(3,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(4,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(5,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(6,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(7,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(8,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(9,   KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(10,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(11,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(12,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(13,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(14,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(15,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(16,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(17,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(18,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(19,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(20,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(32,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(33,  KERNEL_CODE_SEGMENT, GATE_TYPE_0);
	IDT_ENTRY(128, KERNEL_CODE_SEGMENT, GATE_TYPE_3);
	IDT_ENTRY(144, KERNEL_CODE_SEGMENT, GATE_TYPE_2);
}
