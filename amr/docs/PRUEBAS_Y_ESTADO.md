# Pruebas y estado del firmware

## Código incluido en esta versión

La aplicación de `src/main.c` está configurada como una prueba simultánea de cuatro motores:

1. Inicializa reloj, pines, LPUART0 y LPUART6.
2. Comprueba comunicación con los RoboClaw `0x80` y `0x81` leyendo el voltaje de batería.
3. Envía un paro inicial a ambos controladores.
4. Reinicia los encoders de ambos RoboClaw.
5. Ordena `+1000 PPS` a M1 y M2 de ambos controladores con aceleración de `1000 PPS/s`.
6. Monitorea durante 3 s los cuatro encoders y las cuatro velocidades.
7. Ordena paro a los cuatro motores.
8. Si ocurre una falla, intenta detener ambos RoboClaw antes de quedar en el ciclo de error.

## Estado de validación

La presencia de la secuencia en el código significa que la funcionalidad está **implementada en software**, pero no constituye por sí sola evidencia de que la prueba de cuatro motores haya sido aprobada físicamente.

Para cerrar la prueba en el registro de calidad se recomienda conservar:

- Captura o log completo de la terminal.
- Fecha de la prueba.
- Identificación de los dos RoboClaw.
- Correspondencia de A1, A2, B1 y B2 con cada rueda.
- Velocidad observada por cada canal.
- Confirmación de paro de los cuatro motores.
- Observaciones de sentido de giro, errores o alarmas.

## Criterio sugerido para la siguiente validación

- Ambos RoboClaw responden sin error de comunicación.
- Los cuatro canales reciben la referencia.
- Cada encoder cambia de forma coherente con su motor.
- Cada velocidad tiene signo y magnitud coherentes con el comando.
- El paro final lleva las referencias a cero de manera controlada.
