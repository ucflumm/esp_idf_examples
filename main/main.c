#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"

#define LED_BLUE GPIO_NUM_18
#define LED_RED GPIO_NUM_19
#define LED_GREEN GPIO_NUM_21
#define BUTTON GPIO_NUM_17
#define ESP_INTR_FLAG_DEFAULT 0

#define GREEN_LIGHT_ON_TIME_MS 10000
#define BLUE_LIGHT_ON_TIME_MS 3000

SemaphoreHandle_t xButtonSemaphore = NULL;

/*
 * button_isr_handler - interrupt service routine that gives a semaphore when the button is pressed
 * @arg: arguments passed to the ISR
 * IRAM_ATTR: place the function in IRAM which is req for ISR
*/
void IRAM_ATTR button_isr_handler(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

/**
 * setup_gpio - setup the GPIOs for the LEDs and the button
 * set the button to be pulled up so we dont have floating values
*/
void setup_gpio()
{
    //led setup leave red led on unless isr is triggered
    gpio_set_direction(LED_BLUE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);
    gpio_set_level(LED_RED, 1);
    // isr setup
    gpio_set_intr_type(BUTTON, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(BUTTON, button_isr_handler, NULL);
    // semaphore setup
    xButtonSemaphore = xSemaphoreCreateBinary();

}
/**
 * button_task - task that blinks an LED when a button is pressed
 * @arg: arguments passed to the task
 * Turns off the red light and turns on the green light for 10 seconds
 * Turns off the green light and turns on the blue light for 3 seconds
 * I used the blue light as a yellow light because I'm using an RGB LED
 * Then turns off the blue light and turns on the red light (default state)
*/
void button_task(void *arg)
{
    while (1)
    {
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE)
        {
            gpio_set_level(LED_RED, 0);
            
            gpio_set_level(LED_GREEN, 1);
            vTaskDelay(GREEN_LIGHT_ON_TIME_MS / portTICK_PERIOD_MS);
            gpio_set_level(LED_GREEN, 0);
            
            gpio_set_level(LED_BLUE, 1);
            vTaskDelay(BLUE_LIGHT_ON_TIME_MS / portTICK_PERIOD_MS);
            gpio_set_level(LED_BLUE, 0);
            
            gpio_set_level(LED_RED, 1);
            printf("Button pressed\n");
        }

    }
}


/**
 * app_main - main function for rtos
*/
void app_main(void)
{
    setup_gpio();
    xTaskCreate(button_task, "button_task", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
}