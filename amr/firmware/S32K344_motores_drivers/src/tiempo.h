#ifndef TIEMPO_H
#define TIEMPO_H

#include "Std_Types.h"

#define CPU_HZ          160000000UL
#define CICLOS_POR_US   (CPU_HZ / 1000000UL)

void Tiempo_Init(void);
uint32 Tiempo_Ciclos(void);
void Tiempo_EsperarUs(uint32 us);

#endif
