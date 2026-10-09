#ifndef ROBOCLAW_H
#define ROBOCLAW_H

#include "Mcal.h"

#include <stdint.h>
#include <stdbool.h>


#define ROBOCLAW_A_ADDRESS  0x80U
#define ROBOCLAW_B_ADDRESS  0x81U


bool RoboClaw_ResetEncoders(
    uint8_t address
);


bool RoboClaw_SpeedAccelM1(
    uint8_t address,
    uint32_t accel,
    int32_t speed
);


bool RoboClaw_SpeedAccelM2(
    uint8_t address,
    uint32_t accel,
    int32_t speed
);


/*
 * M1 y M2 en el mismo comando.
 */
bool RoboClaw_SpeedAccelM1M2(
    uint8_t address,
    uint32_t accel,
    int32_t speed_m1,
    int32_t speed_m2
);


bool RoboClaw_ReadEncoderM1(
    uint8_t address,
    int32_t *encoder,
    uint8_t *status
);


bool RoboClaw_ReadEncoderM2(
    uint8_t address,
    int32_t *encoder,
    uint8_t *status
);


bool RoboClaw_ReadSpeedM1(
    uint8_t address,
    uint32_t *speed,
    uint8_t *status
);


bool RoboClaw_ReadSpeedM2(
    uint8_t address,
    uint32_t *speed,
    uint8_t *status
);


bool RoboClaw_ReadMainBattery(
    uint8_t address,
    uint16_t *battery_tenths
);


bool RoboClaw_StopAll(
    uint8_t address,
    uint32_t accel
);


#endif
