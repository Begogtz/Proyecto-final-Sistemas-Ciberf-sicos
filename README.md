# Proyecto de Sistemas Ciberfísicos — Software AMR

Este directorio contiene el desarrollo de software del AMR del proyecto de Sistemas Ciberfísicos.

## Estado actual

El firmware disponible está desarrollado para una **FRDM-A-S32K344** y controla dos **RoboClaw 2x45ST** mediante Packet Serial. El código actual está preparado para manejar los dos canales de cada RoboClaw, es decir, hasta cuatro motores.

La aplicación incluida en `main.c` realiza una prueba de cuatro motores con dos controladores configurados en las direcciones `0x80` y `0x81`. Antes de marcar esta funcionalidad como validada en la documentación de calidad, debe ejecutarse físicamente la prueba y conservar evidencia del resultado.

## Estructura

```text
amr/
├── firmware/
│   └── S32K344_motores_drivers/   Proyecto de S32 Design Studio
└── docs/
    ├── ARQUITECTURA.md
    ├── PINOUT_Y_COMUNICACION.md
    ├── PRUEBAS_Y_ESTADO.md
    └── CONTROL_VERSIONES.md
```

## Firmware

El código propio principal se encuentra en:

- `src/main.c`: secuencia de inicialización y prueba.
- `src/roboclaw.c`: implementación de Packet Serial, CRC16 y comandos utilizados con RoboClaw.
- `src/roboclaw.h`: interfaz pública del controlador RoboClaw.
- `src/tiempo.c` y `src/tiempo.h`: temporización mediante DWT.

El proyecto también conserva los archivos de configuración, código generado, RTD y ajustes de S32 Design Studio necesarios para mantener reproducible el proyecto original.

## Comunicación configurada

- **LPUART0**: enlace FRDM-A-S32K344 ↔ RoboClaw, 38400 baud.
- **LPUART6**: salida de depuración para terminal, 115200 baud.

Consultar `amr/docs/PINOUT_Y_COMUNICACION.md` para el detalle.

## Próximos pasos

1. Ejecutar y documentar la prueba física de los cuatro motores.
2. Confirmar sentido físico y correspondencia de cada canal con las ruedas FR, FL, BR y BL.
3. Implementar la capa de cinemática Mecanum.
4. Integrar la comunicación de alto nivel con la Intel NUC.
5. Mantener las pruebas y cambios asociados a commits identificables.



