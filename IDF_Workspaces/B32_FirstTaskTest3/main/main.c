//Beck, main.c, 12/2/23b
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "Tasks.h"

void app_main()
{
	xTaskCreate(&Task1, "Task_1", 2048, NULL, 5, NULL);
	xTaskCreate(&Task2, "Task_2", 2048, NULL, 5, NULL);
	return;
}	//app_main
//Last line
