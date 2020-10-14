/*
 * hugs.c
 *
 *  Created on: Mar 20, 2020
 *      Author: Phil
 */

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "hugs.h"
#include "hugsIO.h"

#define CYCLES_PER_SEC   20
#define FWD_PIN	19
#define REV_PIN 21
#define LFT_PIN 22
#define RIG_PIN 23
#define PIN_SEL   ((1ULL << FWD_PIN) | (1ULL << REV_PIN) | (1ULL << LFT_PIN) | (1ULL << RIG_PIN))

#define TOP_AXIAL_SPEED 230
#define TOP_YAW_SPEED    60
#define TOP_SWEEP_SPEED  20

#define SHIFT_BITS      2
#define AXIAL_ACC_LIMIT 300      //  mm/s/s
#define YAW_ACC_LIMIT   45       //  deg/s/s
#define SWEEP_ACC_LIMIT 30       //  deg/s/s

static void driving(void *pvParameter);

int16_t     targetAxialFP = 0;
int16_t     targetYawFP   = 0;

int16_t     limitedAxialFP = 0;
int16_t     limitedYawFP   = 0;

int16_t     accelAxialFP = 0;
int16_t     accelYawFP   = 0;

int16_t     accelLimitAxialFP   = (AXIAL_ACC_LIMIT << SHIFT_BITS) / CYCLES_PER_SEC ;
int16_t     accelLimitYawFP     = (YAW_ACC_LIMIT   << SHIFT_BITS) / CYCLES_PER_SEC ;
int16_t     accelLimitSweepFP   = (SWEEP_ACC_LIMIT << SHIFT_BITS) / CYCLES_PER_SEC ;

int16_t     topAxialSpeedFP   = (TOP_AXIAL_SPEED << SHIFT_BITS) ;
int16_t     topYawSpeedFP     = (TOP_YAW_SPEED   << SHIFT_BITS) ;
int16_t     topSweepSpeedFP   = (TOP_SWEEP_SPEED << SHIFT_BITS) ;

TickType_t PostPeriod = (1000 / CYCLES_PER_SEC) / portTICK_PERIOD_MS;

bool	goFwd;
bool	goRev;
bool	goLft;
bool	goRig;

void	start_HUGSIO_tasks(){
    // Configure IO Pins
    gpio_config_t io_conf;

	// Setup inputs
    targetAxialFP = 0;
    targetYawFP   = 0;
    limitedAxialFP = 0;
    limitedYawFP   = 0;

	// Pull-up Button pins
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

	// Create polling task.
    xTaskCreate(&driving, "driving task", 2048, NULL, 6, NULL);
}

static void driving(void *pvParameter)
{
    TickType_t 	xLastWakeTime;

    // Initialize the xLastWakeTime variable with the current time.
    resetPosition();
    setSpeedMode(SPEED_PF, 200);
    xLastWakeTime = xTaskGetTickCount();

    while(1) {
    	// Wait one full post cycle
        vTaskDelayUntil( &xLastWakeTime, PostPeriod );

        goFwd =	(gpio_get_level(FWD_PIN) == 0);
        goRev =	(gpio_get_level(REV_PIN) == 0);
        goLft =	(gpio_get_level(LFT_PIN) == 0);
        goRig =	(gpio_get_level(RIG_PIN) == 0);

        // Use the joystick buttons to set the target speeds
        accelAxialFP = accelLimitAxialFP;
        accelYawFP   = accelLimitYawFP;

        if (goFwd)
            targetAxialFP =  topAxialSpeedFP;
        else if (goRev)
            targetAxialFP = -topAxialSpeedFP;
        else
            targetAxialFP =   0;

        if (goRig){
            if (targetAxialFP == 0)
                targetYawFP =  topYawSpeedFP;
            else {
                targetYawFP =  topSweepSpeedFP;
                accelYawFP   = accelLimitSweepFP;
            }
        }
        else if (goLft){
            if (targetAxialFP == 0)
                targetYawFP =  -topYawSpeedFP;
            else {
                targetYawFP =  -topSweepSpeedFP;
                accelYawFP   = accelLimitSweepFP;
            }
        }
        else
            targetYawFP =   0;

        // calculate motion profile and send to serial port.
        calculateMotion();

        setTwist(limitedAxialFP >> SHIFT_BITS, limitedYawFP >> SHIFT_BITS);
    }
}

void    calculateMotion(void) {
	// use target speeds and acceleration limits to generate limited speeds.
	limitedAxialFP = limitSpeed(targetAxialFP, limitedAxialFP, accelAxialFP);
	limitedYawFP   = limitSpeed(targetYawFP,   limitedYawFP,   accelYawFP);
}

int16_t limitSpeed(int16_t targetFP, int16_t limitedFP, int16_t accelFP) {

	int16_t diffFP  = targetFP - limitedFP;
	int16_t speedFP = targetFP;

	// boost deceleration rate if we are stopping
	if (targetFP == 0)
		accelFP *= 2;

	// Ensure acceleration limit is not too small.
	if (accelFP == 0)
		accelFP = 1;

	// Limit acceleration (speed change) if required.
	if (diffFP > accelFP){
		speedFP = limitedFP + accelFP;
	} else if (diffFP < -accelFP){
		speedFP = limitedFP - accelFP;
	}

	return (speedFP);
}

