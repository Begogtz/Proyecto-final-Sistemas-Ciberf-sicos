# Pinout y comunicaciones

La información de esta página corresponde a los archivos de configuración incluidos en el proyecto de S32 Design Studio.

## Bus RoboClaw — LPUART0

| Función | Pin S32K344 | Dirección | Uso |
|---|---|---|---|
| LPUART0_TX | PTA3 | Salida | TX de S32K344 hacia S1 de los RoboClaw |
| LPUART0_RX | PTA2 | Entrada | RX de S32K344 desde S2 de los RoboClaw |

Configuración:

- Baud rate: **38400 baud**.
- RoboClaw A: **0x80**.
- RoboClaw B: **0x81**.

El `main.c` documenta ambos RoboClaw compartiendo el bus serial y diferenciándose por dirección.

## Depuración — LPUART6

| Función | Pin S32K344 | Dirección | Uso |
|---|---|---|---|
| LPUART6_TX | PTA16 | Salida | Mensajes de depuración hacia terminal |
| LPUART6_RX | PTA15 | Entrada | RX de depuración configurado en el proyecto |

Configuración:

- Baud rate: **115200 baud**.
- Uso actual: mensajes enviados por `Debug_Send()` para visualizar la prueba en una terminal como PuTTY.
