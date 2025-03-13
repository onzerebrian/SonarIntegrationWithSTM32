/*
 * sonar.h
 *
 *  Created on: Jan 17, 2025
 *      Author: onzer
 */

#ifndef INC_SONAR_H_
#define INC_SONAR_H_

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim2; // Use the TIM2 handle from tim.c

float ReadSonarSensor(void);
void HAL_DelayMicroseconds(uint16_t micros);

float get_filtered_distance();
float get_median_distance();

#endif /* INC_SONAR_H_ */
