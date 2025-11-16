// bno055.c
#include "bno055.h"
#include "stm32wbxx_hal.h"

static HAL_StatusTypeDef BNO055_Write8(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t val)
{
    return HAL_I2C_Mem_Write(hi2c, BNO055_I2C_ADDR, reg,
                             I2C_MEMADD_SIZE_8BIT, &val, 1, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef BNO055_Read8(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *val)
{
    return HAL_I2C_Mem_Read(hi2c, BNO055_I2C_ADDR, reg,
                            I2C_MEMADD_SIZE_8BIT, val, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef BNO055_Init(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef ret;
    uint8_t id;

    // Read chip ID
    ret = BNO055_Read8(hi2c, BNO055_CHIP_ID_REG, &id);
    if (ret != HAL_OK || id != BNO055_CHIP_ID_VAL)
    {
        return HAL_ERROR;
    }

    // Switch to CONFIG mode
    ret = BNO055_Write8(hi2c, BNO055_OPR_MODE_REG, BNO055_MODE_CONFIG);
    if (ret != HAL_OK) return ret;
    HAL_Delay(25);

    // Make sure we're on page 0
    ret = BNO055_Write8(hi2c, BNO055_PAGE_ID_REG, 0x00);
    if (ret != HAL_OK) return ret;

    // Normal power mode
    ret = BNO055_Write8(hi2c, BNO055_PWR_MODE_REG, BNO055_PWR_MODE_NORMAL);
    if (ret != HAL_OK) return ret;
    HAL_Delay(10);

    // Optional: configure units
    // 0x00 = deg, C, m/s^2, etc.; default is usually fine
    // ret = BNO055_Write8(hi2c, BNO055_UNIT_SEL_REG, 0x00);

    // Start fusion in NDOF mode
    ret = BNO055_Write8(hi2c, BNO055_OPR_MODE_REG, BNO055_MODE_NDOF);
    if (ret != HAL_OK) return ret;
    HAL_Delay(25);

    return HAL_OK;
}

HAL_StatusTypeDef BNO055_ReadEuler(I2C_HandleTypeDef *hi2c, BNO055_Euler_t *euler)
{
    uint8_t buf[6];
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Mem_Read(hi2c,
                           BNO055_I2C_ADDR,
                           BNO055_EULER_H_LSB,
                           I2C_MEMADD_SIZE_8BIT,
                           buf, sizeof(buf),
                           HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;

    int16_t h = (int16_t)((buf[1] << 8) | buf[0]);
    int16_t r = (int16_t)((buf[3] << 8) | buf[2]);
    int16_t p = (int16_t)((buf[5] << 8) | buf[4]);

    // 16 LSB = 1 degree
    euler->heading_deg = h / 16.0f;
    euler->roll_deg    = r / 16.0f;
    euler->pitch_deg   = p / 16.0f;

    return HAL_OK;
}
