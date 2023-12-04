#if 0
	#include <stdio.h>
	#include <stdbool.h>
	#include <unistd.h>

	void app_main(void)
	{
		while (true) {
			printf("Hello from app_main!\n");
			sleep(1);
		}
	}
#else
	//Beck, main.c, 12/3/23c
	#include <stdio.h>
	#include "freertos/FreeRTOS.h"
	#include "freertos/task.h"
	#include "esp_system.h"
	#include "Tasks.h"

	void app_main()
	{
		xTaskCreate(&LaunchTasks, "Launch_Tasks", 2048, NULL, 5, NULL);
		return;
	}	//app_main
	//Last line
#endif
