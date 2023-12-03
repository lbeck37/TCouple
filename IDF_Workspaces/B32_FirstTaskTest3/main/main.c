#if 0
	#include <stdio.h>
	#include <stdbool.h>
	#include <unistd.h>

	void app_main(void)
	{
		while (true) {
			printf("Hello from app_main!\n");
			sleep(2);
		}
	}
#else
	#include <stdio.h>
	#include "freertos/FreeRTOS.h"
	#include "freertos/task.h"
	#include "esp_system.h"

#define TASK_PERIOD_MSEC	2000

	void hello_task(void *pvParameter)
	{
	/*
		printf("Hello world!\n");
		for (int i = 10; i >= 0; i--) {
			printf("Restarting in %d seconds...\n", i);
			vTaskDelay(1000 / portTICK_RATE_MS);
		}
		printf("Restarting now.\n");
		fflush(stdout);
		esp_restart();
	*/
		while(true){
			printf("hello_task(): Hello world!\n");
			//sleep(2);
			vTaskDelay(TASK_PERIOD_MSEC/portTICK_PERIOD_MS);
		}
	}   //hello_task


	void app_main()
	{
		//nvs_flash_init();
		xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
	}
#endif
