# Arquitectura de control del AMR

## Alcance de este firmware

La FRDM-A-S32K344 funciona como controlador de bajo nivel del sistema de tracción. El firmware se comunica con dos RoboClaw 2x45ST y utiliza dos canales por controlador.

```text
Intel NUC / control de alto nivel
           |
           | integración pendiente
           v
     FRDM-A-S32K344
           |
           | LPUART0 / Packet Serial
           | 38400 baud
           v
   +-------------------+
   |                   |
RoboClaw A          RoboClaw B
 dirección 0x80      dirección 0x81
   |       |           |       |
  M1      M2          M1      M2
```

## Funciones implementadas en el código actual

El archivo `roboclaw.c` implementa las operaciones que actualmente utiliza el proyecto:

- CRC16 para Packet Serial.
- Escritura de paquetes con ACK.
- Reinicio de encoders.
- Lectura de encoder M1 y M2.
- Lectura de velocidad M1 y M2.
- Lectura del voltaje de batería principal.
- Comando de velocidad y aceleración para M1.
- Comando de velocidad y aceleración para M2.
- Comando conjunto de velocidad/aceleración para M1 y M2.
- Paro de ambos motores de un RoboClaw mediante referencia de velocidad cero.

## Nota sobre telemetría

El proyecto entregado en este estado **no contiene funciones de lectura de corriente ni lectura de parámetros PID/QPPS**. Si esas funciones existieron en versiones de prueba anteriores, deben incorporarse de nuevo al repositorio antes de documentarlas como parte de esta versión del firmware.

## Pendiente de integración

- Asociación definitiva de canales a FR / FL / BR / BL.
- Convención de signos de cada motor según instalación física.
- Cinemática Mecanum.
- Interfaz NUC ↔ S32K344.
- Manejo de fallas a nivel de sistema.
