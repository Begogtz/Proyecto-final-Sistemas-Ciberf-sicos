#include "roboclaw.h"

#include "Lpuart_Uart_Ip.h"
#include "Lpuart_Uart_Ip_Cfg.h"

#include <stdint.h>
#include <stdbool.h>


#define ROBO_UART_INSTANCE  0U
#define ROBO_UART_TIMEOUT   1000000U


#define CMD_GET_M1_ENCODER       16U
#define CMD_GET_M2_ENCODER       17U

#define CMD_GET_M1_SPEED         18U
#define CMD_GET_M2_SPEED         19U

#define CMD_RESET_ENCODERS       20U

#define CMD_GET_MAIN_BATTERY     24U

#define CMD_M1_SPEED_ACCEL       38U
#define CMD_M2_SPEED_ACCEL       39U

#define CMD_M1_M2_SPEED_ACCEL    40U

#define CMD_GET_M1_VELOCITY_PID  55U
#define CMD_GET_M2_VELOCITY_PID  56U


static uint16_t CRC16_Update(
    uint16_t crc,
    uint8_t data
)
{
    uint8_t i;

    crc ^= ((uint16_t)data << 8U);

    for (i = 0U; i < 8U; i++)
    {
        if ((crc & 0x8000U) != 0U)
        {
            crc =
                (uint16_t)(
                    (crc << 1U) ^
                    0x1021U
                );
        }
        else
        {
            crc <<= 1U;
        }
    }

    return crc;
}


static void PutU32BE(
    uint8_t *buffer,
    uint32_t value
)
{
    buffer[0] =
        (uint8_t)(value >> 24U);

    buffer[1] =
        (uint8_t)(value >> 16U);

    buffer[2] =
        (uint8_t)(value >> 8U);

    buffer[3] =
        (uint8_t)value;
}


static uint32_t GetU32BE(
    const uint8_t *buffer
)
{
    return
        ((uint32_t)buffer[0] << 24U) |
        ((uint32_t)buffer[1] << 16U) |
        ((uint32_t)buffer[2] << 8U) |
        ((uint32_t)buffer[3]);
}


static bool RoboClaw_WritePacket(
    uint8_t address,
    uint8_t command,
    const uint8_t *data,
    uint8_t data_length
)
{
    uint8_t packet[16];

    uint8_t packet_length;
    uint8_t ack = 0U;
    uint8_t i;

    uint16_t crc = 0U;

    Lpuart_Uart_Ip_StatusType uart_status;


    /*
     * El paquete más grande que usamos
     * tiene 12 bytes de datos.
     */
    if (data_length > 12U)
    {
        return false;
    }


    packet[0] = address;
    packet[1] = command;


    for (i = 0U; i < data_length; i++)
    {
        packet[2U + i] = data[i];
    }


    for (
        i = 0U;
        i < (uint8_t)(2U + data_length);
        i++
    )
    {
        crc =
            CRC16_Update(
                crc,
                packet[i]
            );
    }


    packet[2U + data_length] =
        (uint8_t)(crc >> 8U);

    packet[3U + data_length] =
        (uint8_t)crc;


    packet_length =
        (uint8_t)(
            data_length + 4U
        );


    uart_status =
        Lpuart_Uart_Ip_SyncSend(
            ROBO_UART_INSTANCE,
            packet,
            packet_length,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    uart_status =
        Lpuart_Uart_Ip_SyncReceive(
            ROBO_UART_INSTANCE,
            &ack,
            1U,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    return (ack == 0xFFU);
}


static bool RoboClaw_Read4Status(
    uint8_t address,
    uint8_t command,
    uint32_t *value,
    uint8_t *status
)
{
    uint8_t request[2];
    uint8_t response[7];

    uint16_t crc = 0U;
    uint16_t received_crc;

    uint8_t i;

    Lpuart_Uart_Ip_StatusType uart_status;


    request[0] = address;
    request[1] = command;


    uart_status =
        Lpuart_Uart_Ip_SyncSend(
            ROBO_UART_INSTANCE,
            request,
            2U,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    uart_status =
        Lpuart_Uart_Ip_SyncReceive(
            ROBO_UART_INSTANCE,
            response,
            7U,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    crc =
        CRC16_Update(
            crc,
            address
        );

    crc =
        CRC16_Update(
            crc,
            command
        );


    for (i = 0U; i < 5U; i++)
    {
        crc =
            CRC16_Update(
                crc,
                response[i]
            );
    }


    received_crc =
        ((uint16_t)response[5] << 8U) |
        ((uint16_t)response[6]);


    if (crc != received_crc)
    {
        return false;
    }


    if (value != NULL)
    {
        *value =
            GetU32BE(
                response
            );
    }


    if (status != NULL)
    {
        *status =
            response[4];
    }


    return true;
}


static bool RoboClaw_ReadVelocityPid(
    uint8_t address,
    uint8_t command,
    RoboClaw_VelocityPid *pid
)
{
    uint8_t request[2];
    uint8_t response[18];

    uint16_t crc = 0U;
    uint16_t received_crc;

    uint8_t i;

    Lpuart_Uart_Ip_StatusType uart_status;


    if (pid == NULL)
    {
        return false;
    }


    request[0] = address;
    request[1] = command;


    uart_status =
        Lpuart_Uart_Ip_SyncSend(
            ROBO_UART_INSTANCE,
            request,
            2U,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    uart_status =
        Lpuart_Uart_Ip_SyncReceive(
            ROBO_UART_INSTANCE,
            response,
            18U,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    crc =
        CRC16_Update(
            crc,
            address
        );

    crc =
        CRC16_Update(
            crc,
            command
        );


    for (i = 0U; i < 16U; i++)
    {
        crc =
            CRC16_Update(
                crc,
                response[i]
            );
    }


    received_crc =
        ((uint16_t)response[16] << 8U) |
        ((uint16_t)response[17]);


    if (crc != received_crc)
    {
        return false;
    }


    pid->p_raw =
        GetU32BE(
            &response[0]
        );

    pid->i_raw =
        GetU32BE(
            &response[4]
        );

    pid->d_raw =
        GetU32BE(
            &response[8]
        );

    pid->qpps =
        GetU32BE(
            &response[12]
        );


    return true;
}


bool RoboClaw_ReadMainBattery(
    uint8_t address,
    uint16_t *battery_tenths
)
{
    uint8_t request[2];
    uint8_t response[4];

    uint16_t crc = 0U;
    uint16_t received_crc;
    uint16_t value;

    Lpuart_Uart_Ip_StatusType uart_status;


    request[0] = address;
    request[1] = CMD_GET_MAIN_BATTERY;


    uart_status =
        Lpuart_Uart_Ip_SyncSend(
            ROBO_UART_INSTANCE,
            request,
            2U,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    uart_status =
        Lpuart_Uart_Ip_SyncReceive(
            ROBO_UART_INSTANCE,
            response,
            4U,
            ROBO_UART_TIMEOUT
        );


    if (
        uart_status !=
        LPUART_UART_IP_STATUS_SUCCESS
    )
    {
        return false;
    }


    crc =
        CRC16_Update(
            crc,
            address
        );

    crc =
        CRC16_Update(
            crc,
            CMD_GET_MAIN_BATTERY
        );

    crc =
        CRC16_Update(
            crc,
            response[0]
        );

    crc =
        CRC16_Update(
            crc,
            response[1]
        );


    received_crc =
        ((uint16_t)response[2] << 8U) |
        ((uint16_t)response[3]);


    if (crc != received_crc)
    {
        return false;
    }


    value =
        ((uint16_t)response[0] << 8U) |
        ((uint16_t)response[1]);


    if (battery_tenths != NULL)
    {
        *battery_tenths =
            value;
    }


    return true;
}


bool RoboClaw_ResetEncoders(
    uint8_t address
)
{
    return RoboClaw_WritePacket(
        address,
        CMD_RESET_ENCODERS,
        NULL,
        0U
    );
}


bool RoboClaw_SpeedAccelM1(
    uint8_t address,
    uint32_t accel,
    int32_t speed
)
{
    uint8_t data[8];


    PutU32BE(
        &data[0],
        accel
    );


    PutU32BE(
        &data[4],
        (uint32_t)speed
    );


    return RoboClaw_WritePacket(
        address,
        CMD_M1_SPEED_ACCEL,
        data,
        8U
    );
}


bool RoboClaw_SpeedAccelM2(
    uint8_t address,
    uint32_t accel,
    int32_t speed
)
{
    uint8_t data[8];


    PutU32BE(
        &data[0],
        accel
    );


    PutU32BE(
        &data[4],
        (uint32_t)speed
    );


    return RoboClaw_WritePacket(
        address,
        CMD_M2_SPEED_ACCEL,
        data,
        8U
    );
}


/*
 * Command 40:
 *
 * Accel
 * Speed M1
 * Speed M2
 *
 * M1 y M2 comparten la misma
 * aceleración.
 */
bool RoboClaw_SpeedAccelM1M2(
    uint8_t address,
    uint32_t accel,
    int32_t speed_m1,
    int32_t speed_m2
)
{
    uint8_t data[12];


    PutU32BE(
        &data[0],
        accel
    );


    PutU32BE(
        &data[4],
        (uint32_t)speed_m1
    );


    PutU32BE(
        &data[8],
        (uint32_t)speed_m2
    );


    return RoboClaw_WritePacket(
        address,
        CMD_M1_M2_SPEED_ACCEL,
        data,
        12U
    );
}


bool RoboClaw_ReadEncoderM1(
    uint8_t address,
    int32_t *encoder,
    uint8_t *status
)
{
    uint32_t raw_value;

    bool ok;


    ok =
        RoboClaw_Read4Status(
            address,
            CMD_GET_M1_ENCODER,
            &raw_value,
            status
        );


    if (ok && (encoder != NULL))
    {
        *encoder =
            (int32_t)raw_value;
    }


    return ok;
}


bool RoboClaw_ReadEncoderM2(
    uint8_t address,
    int32_t *encoder,
    uint8_t *status
)
{
    uint32_t raw_value;

    bool ok;


    ok =
        RoboClaw_Read4Status(
            address,
            CMD_GET_M2_ENCODER,
            &raw_value,
            status
        );


    if (ok && (encoder != NULL))
    {
        *encoder =
            (int32_t)raw_value;
    }


    return ok;
}


bool RoboClaw_ReadSpeedM1(
    uint8_t address,
    uint32_t *speed,
    uint8_t *status
)
{
    return RoboClaw_Read4Status(
        address,
        CMD_GET_M1_SPEED,
        speed,
        status
    );
}


bool RoboClaw_ReadSpeedM2(
    uint8_t address,
    uint32_t *speed,
    uint8_t *status
)
{
    return RoboClaw_Read4Status(
        address,
        CMD_GET_M2_SPEED,
        speed,
        status
    );
}


bool RoboClaw_ReadVelocityPidM1(
    uint8_t address,
    RoboClaw_VelocityPid *pid
)
{
    return RoboClaw_ReadVelocityPid(
        address,
        CMD_GET_M1_VELOCITY_PID,
        pid
    );
}


bool RoboClaw_ReadVelocityPidM2(
    uint8_t address,
    RoboClaw_VelocityPid *pid
)
{
    return RoboClaw_ReadVelocityPid(
        address,
        CMD_GET_M2_VELOCITY_PID,
        pid
    );
}


bool RoboClaw_StopAll(
    uint8_t address,
    uint32_t accel
)
{
    return RoboClaw_SpeedAccelM1M2(
        address,
        accel,
        0,
        0
    );
}
