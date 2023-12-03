//Beck, Tasks.cpp, 12/2/23k
#include "Tasks.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define TASK1_PERIOD_MSEC	2000
#define TASK2_PERIOD_MSEC	6000

#if 1
#ifdef __cplusplus
	extern "C" {
#endif

void Task1(void *pvParameter){
	while(true){
		printf("Task1(): Hello World! 12/2/23k\n");
		vTaskDelay(TASK1_PERIOD_MSEC/portTICK_PERIOD_MS);
	}	//while(true)
  //It's a task so it should never return
}   //Task1

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
	extern "C" {
#endif

void Task2(void *pvParameter){
	while(true){
		printf("Task2(): It's my turn!\n");
		vTaskDelay(TASK2_PERIOD_MSEC/portTICK_PERIOD_MS);
	}	//while(true)
  //It's a task so it should never return
}   //Task2

#ifdef __cplusplus
}
#endif
#else
	//Function protos
	void Task1		(void *pvParameter);
	void Task2		(void *pvParameter);

	#ifdef __cplusplus
		extern "C" {
	#endif
	void LaunchTasks	(void *pvParameter){
/*
		Task1(pvParameter);
		Task2(pvParameter);
*/
		xTaskCreate(&Task1, "Task_One", 2048, NULL, 5, NULL);
		xTaskCreate(&Task2, "Task_Two", 2048, NULL, 5, NULL);
	}
	#ifdef __cplusplus
	}
	#endif


	void Task1(void *pvParameter){
		while(true){
			printf("Task1(): Hello World! 12/2/23j\n");
			vTaskDelay(TASK1_PERIOD_MSEC/portTICK_PERIOD_MS);
		}	//while(true)
	  //It's a task so it should never return
	}   //Task1


	void Task2(void *pvParameter){
		while(true){
			printf("Task2(): It's my turn!\n");
			vTaskDelay(TASK2_PERIOD_MSEC/portTICK_PERIOD_MS);
		}	//while(true)
	  //It's a task so it should never return
	}   //Task2
#endif
//Last line
