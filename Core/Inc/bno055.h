// bno055.h
#ifndef BNO055_H
#define BNO055_H

#include "stm32wbxx_hal.h"

// Default I2C address when COM3 pin is low
#define BNO055_I2C_ADDR           (0x29 << 1)

// Key registers
#define BNO055_CHIP_ID_REG        0x00
#define BNO055_CHIP_ID_VAL        0xA0

#define BNO055_PAGE_ID_REG        0x07
#define BNO055_UNIT_SEL_REG       0x3B
#define BNO055_OPR_MODE_REG       0x3D
#define BNO055_PWR_MODE_REG       0x3E
#define BNO055_SYS_TRIGGER_REG    0x3F

// Operation modes
#define BNO055_MODE_CONFIG        0x00
#define BNO055_MODE_NDOF          0x0C

// Power modes
#define BNO055_PWR_MODE_NORMAL    0x00

// Euler angle registers (heading, roll, pitch), 16 LSB = 1 deg
#define BNO055_EULER_H_LSB        0x1A  // H LSB, then H MSB, R LSB/MSB, P LSB/MSB

typedef struct
{
    float heading_deg;  // yaw
    float roll_deg;
    float pitch_deg;
} BNO055_Euler_t;

HAL_StatusTypeDef BNO055_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BNO055_ReadEuler(I2C_HandleTypeDef *hi2c, BNO055_Euler_t *euler);

#endif // BNO055_H
