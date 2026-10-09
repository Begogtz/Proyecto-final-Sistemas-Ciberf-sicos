\# Riel lineal con servo Delta — Estado actual



\## 1. Objetivo



El sistema de riel lineal forma parte de la celda ciberfísica del proyecto y tiene como objetivo desplazar un equipo montado sobre el riel entre diferentes posiciones de trabajo previamente definidas.



El movimiento será realizado mediante un servomotor Delta y su servo drive. El sistema deberá permitir desplazamientos controlados y repetibles hacia posiciones asociadas con las diferentes estaciones de la celda.



\## 2. Hardware considerado



Actualmente se cuenta con:



\- Servomotor Delta.

\- Servo drive Delta.

\- Riel lineal.

\- Sistema de control del proyecto.

\- Sensores o señales digitales para indicar las posiciones o comandos de movimiento.



El modelo exacto y las características eléctricas finales del servomotor y del drive deberán quedar registrados una vez que se confirme la información de placa de ambos equipos.



\## 3. Software de configuración



Para la configuración del servo drive se utiliza:



\- \*\*ASDA-Soft V5.5.0\*\*



Este software permite establecer comunicación con el drive, consultar y modificar parámetros, monitorear variables y realizar pruebas de movimiento del servomotor.



\## 4. Estrategia de control prevista



El sistema se plantea como un control de posicionamiento con posiciones discretas previamente definidas.



La secuencia general esperada es:



1\. El sistema recibe una señal o condición de movimiento.

2\. Se identifica la posición objetivo correspondiente.

3\. El controlador envía la referencia al servo drive Delta.

4\. El servomotor desplaza el riel hacia la posición solicitada.

5\. Se verifica que la posición objetivo haya sido alcanzada.

6\. El sistema queda disponible para recibir un nuevo comando.



Las posiciones deberán ser definidas y calibradas durante la etapa de integración.



\## 5. Interfaz con sensores y señales



Se contempla utilizar sensores o señales digitales para coordinar el desplazamiento del riel con el resto de la celda.



Estas señales podrán utilizarse para:



\- Solicitar movimiento hacia una posición específica.

\- Confirmar condiciones previas al movimiento.

\- Detectar límites o posiciones de referencia.

\- Confirmar llegada a una estación.

\- Evitar movimientos cuando no existan condiciones seguras.



La asignación definitiva de entradas y salidas todavía se encuentra pendiente.



\## 6. Estado actual



\*\*Estado: En proceso\*\*



Actualmente se está realizando la etapa inicial de configuración y aprendizaje del sistema Delta utilizando ASDA-Soft V5.5.0.



Se ha identificado la necesidad de configurar el sistema para operación por posicionamiento y posteriormente establecer las posiciones fijas requeridas por la celda.



Aún no se considera validado el movimiento automático completo del riel ni la activación por sensores.



\## 7. Pruebas previstas



Las siguientes pruebas deberán realizarse durante el desarrollo:



| Prueba | Estado |

|---|---|

| Comunicación PC–servo drive mediante ASDA-Soft | En proceso |

| Lectura y respaldo de parámetros del drive | Pendiente |

| Movimiento manual controlado del servomotor | Pendiente |

| Validación del sentido de movimiento | Pendiente |

| Configuración del modo de posicionamiento | Pendiente |

| Definición de límites de desplazamiento | Pendiente |

| Definición de posiciones de trabajo | Pendiente |

| Movimiento hacia una posición fija | Pendiente |

| Repetibilidad de posicionamiento | Pendiente |

| Integración de señales o sensores | Pendiente |

| Validación de paro y condiciones de seguridad | Pendiente |

| Prueba de secuencia automática completa | Pendiente |



\## 8. Pendientes técnicos



Antes de considerar terminado el subsistema se requiere:



\- Confirmar modelo exacto del servomotor y servo drive.

\- Documentar los datos de placa de ambos dispositivos.

\- Establecer comunicación confiable con ASDA-Soft.

\- Respaldar los parámetros originales del servo drive.

\- Definir el modo de control utilizado.

\- Configurar aceleración y desaceleración.

\- Definir límites de desplazamiento.

\- Determinar las posiciones físicas requeridas.

\- Definir las entradas y salidas utilizadas.

\- Integrar los sensores o señales de control.

\- Implementar las condiciones de seguridad.

\- Validar la repetibilidad de cada posición.

\- Documentar los resultados de las pruebas.



\## 9. Criterio para considerar el subsistema validado



El riel se considerará funcional cuando pueda desplazarse de manera controlada y repetible entre las posiciones definidas, responder correctamente a las señales de control y detenerse de forma segura ante una condición de paro o fallo.



Todas las pruebas deberán quedar respaldadas mediante evidencia y registradas en el formato de control de calidad del proyecto.

