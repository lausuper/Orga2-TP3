/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "screen.h"
#include "sched.h"
#include "isr.h"
#include "gdt.h"

//extern jmpToTask

unsigned short tareas[CANT_TAREAS];
char 		   arbitro			= FALSE;
unsigned short posArbitro 		= 112;
unsigned short posicion 		= 0;

char		   pausarReanudar	= 1;
char		   pausado			= 0;
char		   quantum			= 2;
char		   finalizado		= 0;
char 		   contador			= 0;

void sched_inicializar() {
	tareas[0] = 80;
	tareas[1] = 88;
	tareas[2] = 96;
	tareas[3] = 104;
}

//pausado TRUE & pausarReanudar TRUE -> pausado FALSE
//pausado TRUE & pausarReanudar FALSE -> pausado TRUE
//pausado FALSE & pausarReanudar TRUE -> pausado FALSE
//pausado FALSE & pausarReanudar FALSE -> pausado TRUE

void sched() {
	if(finalizado == 1) return;
	if(quantum == 0) {
		quantum = 2;
		if(pausado == 0 && pausarReanudar == 0) { // hay que pausar 
			pausado 		= 1;
			jmpToTask(72); 	// Salto a la tarea idle
		} else if((pausado == 1 && pausarReanudar == 1) || 
					(pausado == 0 && pausarReanudar == 1)) { // hay que reanudar 
			pausado = 0;
			unsigned short proxTarea = sched_proximo_indice();
			if(proxTarea != -1)	{
				jmpToTask(proxTarea); // salto a la proxima tarea
				breakpoint();
			} else {
				finalizado = 1;
				jmpToTask(72); 	// Se terminó todo, salto a idle
			} 
		}
	}
	else {
		quantum--;
	}
return;


	/*
	if(finalizado == 1) return;

	if(quantum == 0) {
		quantum = 2;
		if(pausado == 0 && pausarReanudar == 1) {
			pausado = 1;
			jmpToTask(72); //Salto a la tarea idle
			return;
		}
		else if(pausado == 1 && pausarReanudar == 0) {
			pausado = 0;
		}

		unsigned short proxTarea = sched_proximo_indice();
		if(proxTarea != -1) { //Si hay tareas activas

			jmpToTask(proxTarea);
		} else {
			finalizado = 1;
			jmpToTask(72); //Salto a la tarea idle
		}
	}
	quantum--;
	return;*/ 
}

unsigned short sched_proximo_indice() {
	unsigned short 	result;
	unsigned short	cant 	= 0;

	if (arbitro == FALSE) {
		result 	= posArbitro;
		arbitro = TRUE;
	} else {
		while (tareas[posicion] == 0 && cant < 4) {
			++posicion;
			++cant;
		}
		if(cant == 4) {
			return -1;
		}
		result 	= tareas[posicion];
		arbitro = FALSE;
		++posicion;
	} 
	return result;
}

void sched_remover_tarea(unsigned int process_id) {
	tareas[process_id] = 0;
}

//Tener en cuenta que devuelve la posición en la GDT de la tarea
//Para tener la posición en tareas[] hay que restarle 10 ya que empieza a contar desde la idle
//y la idle no está en tareas[]
unsigned short tareaActiva() {
	int i;
	unsigned short task = 0;
	for(i = 9; i < 15; i++) {
		unsigned short busy = gdt[i].type;
		busy = busy & 0x0002; //Dejo únicamente el bit de busy
		if(busy == 2) task = i;
	}
	return task;
}