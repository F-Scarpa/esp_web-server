#ifndef PININIT_H
#define PININIT_H

#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "cJSON.h"

#define myPin 2
#define redLed 12
#define blueLed 13
#define greenLed 14


void pinInit(void);

#endif