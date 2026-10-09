#include "tiempo.h"

#define DEMCR               (*(volatile uint32 *)0xE000EDFCUL)
#define DEMCR_TRCENA        (1UL << 24U)

#define DWT_CTRL            (*(volatile uint32 *)0xE0001000UL)
#define DWT_CTRL_CYCCNTENA  (1UL << 0U)

#define DWT_CYCCNT          (*(volatile uint32 *)0xE0001004UL)

#define DWT_LAR             (*(volatile uint32 *)0xE0001FB0UL)
#define DWT_LAR_CLAVE       0xC5ACCE55UL


void Tiempo_Init(void)
{
    DEMCR |= DEMCR_TRCENA;

    DWT_LAR = DWT_LAR_CLAVE;

    DWT_CYCCNT = 0U;

    DWT_CTRL |= DWT_CTRL_CYCCNTENA;
}


uint32 Tiempo_Ciclos(void)
{
    return DWT_CYCCNT;
}


void Tiempo_EsperarUs(uint32 us)
{
    uint32 inicio;
    uint32 espera;

    inicio = DWT_CYCCNT;

    espera = us * CICLOS_POR_US;

    while ((uint32)(DWT_CYCCNT - inicio) < espera)
    {
    }
}
