/*
 * sonar.c
 *
 *  Created on: Jan 17, 2025
 *      Author: onzer
 */


#include "sonar.h"
#include "gpio.h"
#include <stdio.h>
#include "i2c-lcd.h"

/* Microsecond delay using TIM2 */
void HAL_DelayMicroseconds(uint16_t micros)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0); // Reset counter
    while (__HAL_TIM_GET_COUNTER(&htim2) < micros); // Wait
}

/* Read distance from the ultrasonic sensor in cm */
float ReadSonarSensor(void)
{
//    uint32_t start_time, end_time;
//    float distance;
//
//    // 1) Trigger the sensor with a 10 µs pulse on TRIG pin
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // TRIG HIGH
//    HAL_DelayMicroseconds(10); // Wait 10 µs
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // TRIG LOW
//
//    // 2) Wait for ECHO pin to go HIGH
//    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
//    {
//        // Optionally, add a timeout to avoid infinite blocking
//    }
//    start_time = __HAL_TIM_GET_COUNTER(&htim2);
//
//    // 3) Wait for ECHO pin to go LOW
//    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
//    {
//        // Optionally, add a timeout to avoid infinite blocking
//    }
//    end_time = __HAL_TIM_GET_COUNTER(&htim2);
//
//    // 4) Calculate duration and convert to distance
//    uint32_t delta = (end_time >= start_time) ?
//                     (end_time - start_time) :
//                     ((65535 - start_time) + end_time);
//
//    // Convert time duration (µs) to distance (cm)
//    distance = (delta * 0.0343f) / 2.0f;
//
//    return distance; // Distance in cm




//
//	 uint32_t start_time, end_time;
//	    float distance;
//
//	    // 1) Trigger the sensor with a 10 µs pulse
//	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // TRIG HIGH
//	    HAL_DelayMicroseconds(10);                         // Wait 10 µs
//	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // TRIG LOW
//
//	    // 2) Wait for ECHO pin to go HIGH
//	    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
//	    {
//	        // Optional: add timeout to avoid infinite blocking
//	    }
//	    start_time = __HAL_TIM_GET_COUNTER(&htim2);
//
//	    // 3) Wait for ECHO pin to go LOW
//	    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
//	    {
//	        // Optional: add timeout to avoid infinite blocking
//	    }
//	    end_time = __HAL_TIM_GET_COUNTER(&htim2);
//
//	    // 4) Calculate ECHO duration
//	    uint32_t delta = (end_time >= start_time) ?
//	                     (end_time - start_time) :
//	                     ((65535 - start_time) + end_time);
//
//	    // 5) Print raw ECHO duration for debugging
//	    char debug_buffer[16];
//	    sprintf(debug_buffer, "Echo: %lu", delta);
//	    lcd_clear();
//	    lcd_put_cur(0, 0);
//	    lcd_send_string(debug_buffer);
//	    HAL_Delay(1000); // 1 second delay for debugging
//
//	    // 6) Convert to distance (cm)
//	    distance = (delta * 0.0343) / 2.0;
//
//	    return distance; // Distance in cm


	uint32_t start_time, end_time, delta;
	    float distance;

	    // 1) Trigger the sensor with a 10 µs pulse
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	    HAL_DelayMicroseconds(10);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

	    // 2) Wait for ECHO pin to go HIGH
	    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET);
	    start_time = __HAL_TIM_GET_COUNTER(&htim2);

	    // 3) Wait for ECHO pin to go LOW
	    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET);
	    end_time = __HAL_TIM_GET_COUNTER(&htim2);

	    // 4) Calculate ECHO duration (in µs)
	    delta = (end_time >= start_time) ?
	            (end_time - start_time) :
	            ((65535 - start_time) + end_time);

	    // Convert delta to distance (cm)
	    distance = (delta * 0.0343f) / 2.0f;

	    // Debugging: Print raw ECHO duration and calculated distance
//	    char debug_buffer[16];
//
//	    sprintf(debug_buffer, "Dist: %.2f", distance);
//	    lcd_clear();
//	    lcd_put_cur(0, 0);
//	    lcd_send_string(debug_buffer);
//	    HAL_Delay(500); // Pause for debugging

	    return distance;
}


float get_filtered_distance() {
    float sum = 0;
    int num_samples = 5;  // Take 10 samples

    for (int i = 0; i < num_samples; i++) {
        sum += ReadSonarSensor();  // Call your sonar function
        HAL_Delay(800);

    }

    return sum / num_samples;  // Return the average distance
}

float get_median_distance() {
    float samples[5];
    char buffer[16];
    float distance = 0.0f;

    for (int i = 0; i < 5; i++) {
        distance = ReadSonarSensor();
        samples[i] = distance;

        				lcd_put_cur(1, 0);
        				lcd_send_string("Dist(R):");
        	              lcd_put_cur(1, 8);
        	              sprintf(buffer, "%.2fcm", distance);
        	              lcd_send_string(buffer);
        HAL_Delay(800);

    }

    // Sort the array
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (samples[i] > samples[j]) {
                float temp = samples[i];
                samples[i] = samples[j];
                samples[j] = temp;
            }
        }
    }

    return samples[2];  // Return the median value
}
