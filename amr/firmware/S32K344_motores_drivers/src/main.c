#include "Mcal.h"

#include "Clock_Ip.h"
#include "Clock_Ip_Cfg.h"

#include "Siul2_Port_Ip.h"
#include "Siul2_Port_Ip_Cfg.h"

#include "Lpuart_Uart_Ip.h"
#include "Lpuart_Uart_Ip_Cfg.h"

#include "roboclaw.h"
#include "tiempo.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


#define ROBO_UART_INSTANCE      0U
#define DEBUG_UART_INSTANCE     6U


#define TEST_SPEED_PPS          1000
#define TEST_ACCEL_PPS2         1000U

#define TEST_TIME_MS            3000U
#define PRINT_PERIOD_MS         250U


static void Tiempo_EsperarMs(
    uint32_t ms
)
{
    while (ms > 0U)
    {
        Tiempo_EsperarUs(
            1000U
        );

        ms--;
    }
}


static void Debug_Send(
    const char *text
)
{
    (void)Lpuart_Uart_Ip_SyncSend(
        DEBUG_UART_INSTANCE,
        (const uint8_t *)text,
        (uint32_t)strlen(text),
        1000000U
    );
}


static int32_t SpeedSigned(
    uint32_t speed,
    uint8_t status
)
{
    if ((status & 0x01U) != 0U)
    {
        return -(int32_t)speed;
    }

    return (int32_t)speed;
}


static bool Mostrar4Motores(
    uint32_t numero
)
{
    int32_t enc_a1 = 0;
    int32_t enc_a2 = 0;
    int32_t enc_b1 = 0;
    int32_t enc_b2 = 0;

    uint32_t vel_a1 = 0U;
    uint32_t vel_a2 = 0U;
    uint32_t vel_b1 = 0U;
    uint32_t vel_b2 = 0U;

    uint8_t se_a1 = 0U;
    uint8_t se_a2 = 0U;
    uint8_t se_b1 = 0U;
    uint8_t se_b2 = 0U;

    uint8_t sv_a1 = 0U;
    uint8_t sv_a2 = 0U;
    uint8_t sv_b1 = 0U;
    uint8_t sv_b2 = 0U;

    bool ok;

    char msg[300];


    ok =
        RoboClaw_ReadEncoderM1(
            ROBOCLAW_A_ADDRESS,
            &enc_a1,
            &se_a1
        );

    ok =
        ok &&
        RoboClaw_ReadEncoderM2(
            ROBOCLAW_A_ADDRESS,
            &enc_a2,
            &se_a2
        );

    ok =
        ok &&
        RoboClaw_ReadEncoderM1(
            ROBOCLAW_B_ADDRESS,
            &enc_b1,
            &se_b1
        );

    ok =
        ok &&
        RoboClaw_ReadEncoderM2(
            ROBOCLAW_B_ADDRESS,
            &enc_b2,
            &se_b2
        );


    ok =
        ok &&
        RoboClaw_ReadSpeedM1(
            ROBOCLAW_A_ADDRESS,
            &vel_a1,
            &sv_a1
        );

    ok =
        ok &&
        RoboClaw_ReadSpeedM2(
            ROBOCLAW_A_ADDRESS,
            &vel_a2,
            &sv_a2
        );

    ok =
        ok &&
        RoboClaw_ReadSpeedM1(
            ROBOCLAW_B_ADDRESS,
            &vel_b1,
            &sv_b1
        );

    ok =
        ok &&
        RoboClaw_ReadSpeedM2(
            ROBOCLAW_B_ADDRESS,
            &vel_b2,
            &sv_b2
        );


    if (!ok)
    {
        Debug_Send(
            "ERROR leyendo telemetria\r\n"
        );

        return false;
    }


    (void)snprintf(
        msg,
        sizeof(msg),

        "%02lu | "
        "A1=%ld A2=%ld | "
        "B1=%ld B2=%ld | "
        "Enc=%ld,%ld,%ld,%ld\r\n",

        (unsigned long)numero,

        (long)SpeedSigned(
            vel_a1,
            sv_a1
        ),

        (long)SpeedSigned(
            vel_a2,
            sv_a2
        ),

        (long)SpeedSigned(
            vel_b1,
            sv_b1
        ),

        (long)SpeedSigned(
            vel_b2,
            sv_b2
        ),

        (long)enc_a1,
        (long)enc_a2,
        (long)enc_b1,
        (long)enc_b2
    );


    Debug_Send(msg);


    return true;
}


int main(void)
{
    uint16_t battery_a = 0U;
    uint16_t battery_b = 0U;

    uint32_t i;

    char msg[180];


    Clock_Ip_Init(
        &Clock_Ip_aClockConfig[0]
    );


    Siul2_Port_Ip_Init(
        NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
        g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals
    );


    /*
     * RoboClaw bus:
     *
     * PTA3 -> S1 de ambos
     * PTA2 <- S2 de ambos
     *
     * 38400 baud
     */
    Lpuart_Uart_Ip_Init(
        ROBO_UART_INSTANCE,
        &Lpuart_Uart_Ip_xHwConfigPB_0
    );


    /*
     * PuTTY.
     */
    Lpuart_Uart_Ip_Init(
        DEBUG_UART_INSTANCE,
        &Lpuart_Uart_Ip_xHwConfigPB_6
    );


    Tiempo_Init();


    Tiempo_EsperarMs(
        1000U
    );


    Debug_Send(
        "\r\n"
        "========================================\r\n"
        "PRUEBA SIMULTANEA DE 4 MOTORES\r\n"
        "RoboClaw 0x80 + RoboClaw 0x81\r\n"
        "========================================\r\n"
    );


    /*
     * Primero comprobar comunicación
     * con ambos controladores.
     */
    if (
        !RoboClaw_ReadMainBattery(
            ROBOCLAW_A_ADDRESS,
            &battery_a
        )
        ||
        !RoboClaw_ReadMainBattery(
            ROBOCLAW_B_ADDRESS,
            &battery_b
        )
    )
    {
        Debug_Send(
            "ERROR comunicacion inicial\r\n"
        );

        goto error;
    }


    (void)snprintf(
        msg,
        sizeof(msg),

        "Bateria A = %u.%u V\r\n"
        "Bateria B = %u.%u V\r\n",

        (unsigned int)(
            battery_a / 10U
        ),

        (unsigned int)(
            battery_a % 10U
        ),

        (unsigned int)(
            battery_b / 10U
        ),

        (unsigned int)(
            battery_b % 10U
        )
    );


    Debug_Send(msg);


    /*
     * STOP inicial.
     */
    if (
        !RoboClaw_StopAll(
            ROBOCLAW_A_ADDRESS,
            TEST_ACCEL_PPS2
        )
        ||
        !RoboClaw_StopAll(
            ROBOCLAW_B_ADDRESS,
            TEST_ACCEL_PPS2
        )
    )
    {
        Debug_Send(
            "ERROR STOP inicial\r\n"
        );

        goto error;
    }


    Debug_Send(
        "STOP inicial OK\r\n"
    );


    Tiempo_EsperarMs(
        500U
    );


    /*
     * Reiniciar encoders.
     */
    if (
        !RoboClaw_ResetEncoders(
            ROBOCLAW_A_ADDRESS
        )
        ||
        !RoboClaw_ResetEncoders(
            ROBOCLAW_B_ADDRESS
        )
    )
    {
        Debug_Send(
            "ERROR reset encoders\r\n"
        );

        goto error;
    }


    Debug_Send(
        "Encoders = 0\r\n"
    );


    Tiempo_EsperarMs(
        300U
    );


    Debug_Send(
        "\r\n"
        "4 motores -> +1000 PPS\r\n"
        "Accel = 1000 PPS/s\r\n"
        "\r\n"
    );


    /*
     * M1 y M2 del RoboClaw A
     * reciben el objetivo juntos.
     */
    if (
        !RoboClaw_SpeedAccelM1M2(
            ROBOCLAW_A_ADDRESS,
            TEST_ACCEL_PPS2,
            TEST_SPEED_PPS,
            TEST_SPEED_PPS
        )
    )
    {
        Debug_Send(
            "ERROR comando RoboClaw A\r\n"
        );

        goto error;
    }


    /*
     * Unos milisegundos después se
     * actualizan M1 y M2 del RoboClaw B.
     */
    if (
        !RoboClaw_SpeedAccelM1M2(
            ROBOCLAW_B_ADDRESS,
            TEST_ACCEL_PPS2,
            TEST_SPEED_PPS,
            TEST_SPEED_PPS
        )
    )
    {
        Debug_Send(
            "ERROR comando RoboClaw B\r\n"
        );

        goto error;
    }


    /*
     * Monitorear 3 segundos.
     */
    for (
        i = 0U;
        i < (
            TEST_TIME_MS /
            PRINT_PERIOD_MS
        );
        i++
    )
    {
        Tiempo_EsperarMs(
            PRINT_PERIOD_MS
        );


        if (
            !Mostrar4Motores(
                i + 1U
            )
        )
        {
            goto error;
        }
    }


    /*
     * Frenar los cuatro.
     */
    Debug_Send(
        "\r\nSTOP 4 motores\r\n"
    );


    if (
        !RoboClaw_StopAll(
            ROBOCLAW_A_ADDRESS,
            TEST_ACCEL_PPS2
        )
    )
    {
        goto error;
    }


    if (
        !RoboClaw_StopAll(
            ROBOCLAW_B_ADDRESS,
            TEST_ACCEL_PPS2
        )
    )
    {
        goto error;
    }


    Tiempo_EsperarMs(
        1500U
    );


    (void)Mostrar4Motores(
        99U
    );


    Debug_Send(
        "\r\n"
        "========================================\r\n"
        "PRUEBA TERMINADA\r\n"
        "========================================\r\n"
    );


    while (1)
    {
        Tiempo_EsperarMs(
            1000U
        );
    }


error:

    /*
     * Intentar detener todo sin importar
     * en qué punto falló la prueba.
     */
    (void)RoboClaw_StopAll(
        ROBOCLAW_A_ADDRESS,
        TEST_ACCEL_PPS2
    );


    (void)RoboClaw_StopAll(
        ROBOCLAW_B_ADDRESS,
        TEST_ACCEL_PPS2
    );


    Debug_Send(
        "\r\n"
        "ERROR EN PRUEBA\r\n"
        "STOP enviado a ambos RoboClaw\r\n"
    );


    while (1)
    {
        Tiempo_EsperarMs(
            1000U
        );
    }
}
