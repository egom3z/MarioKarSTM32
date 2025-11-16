/*
 * fpu.c
 *
 *  Created on: Nov 15, 2025
 *      Author: enriquegomez
 */
#include "stm32wbxx.h"

void enable_fpu(void)
{
	SCB->CPACR |= (0xF << 20);
}
