#include "pinInit.h"


void pinInit(void)
{
    gpio_set_direction(myPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(redLed, GPIO_MODE_OUTPUT);
    gpio_set_direction(blueLed, GPIO_MODE_OUTPUT);
    gpio_set_direction(greenLed, GPIO_MODE_OUTPUT);

    ledc_timer_config_t timer = {
      .speed_mode = LEDC_LOW_SPEED_MODE,            //speed = hardware slow = software
      .duty_resolution = LEDC_TIMER_10_BIT,         //duty cycle = divide frequency by this amount (10bit )
      .timer_num = LEDC_TIMER_0,                    
      .freq_hz = 5000,          //hz
      .clk_cfg = LEDC_AUTO_CLK  //simple configuration for clock
    };
    ledc_timer_config(&timer);   

    
    ledc_channel_config_t channel = {
      .gpio_num = redLed,                            //output pin
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel = LEDC_CHANNEL_0,                    
      .timer_sel = LEDC_TIMER_0,                //timer we configured before
      .duty = 0,                                //how much of a pulse we want to drive
                                                //duty cycle range depends on timer's duty resolution
                                                // (0 - 1024 for 10 bits)

      .hpoint = 0               //when duty cycle trigger, classic PWM = hpoint 0
    };
    ledc_channel_config(&channel);



    
    gpio_set_level(myPin,false);
}