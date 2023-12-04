//Beck, main.c, 12/3/23b
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "Tasks.h"

#if 0
	void app_main()
	{
		xTaskCreate(&Task1, "Task_1", 2048, NULL, 5, NULL);
		xTaskCreate(&Task2, "Task_2", 2048, NULL, 5, NULL);
		return;
	}	//app_main
#else
	void app_main()
	{
		xTaskCreate(&LaunchTasks, "Launch_Tasks", 2048, NULL, 5, NULL);
		return;
	}	//app_main
#endif
//Last line
