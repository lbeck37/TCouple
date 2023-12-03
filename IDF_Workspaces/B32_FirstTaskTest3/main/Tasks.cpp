//Beck, Tasks.cpp, 12/2/23f
#include "Tasks.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define TASK1_PERIOD_MSEC	2000
#define TASK2_PERIOD_MSEC	6000

#ifdef __cplusplus
	extern "C" {
#endif

void Task1(void *pvParameter){
	while(true){
		printf("Task1(): Hello World! 12/2/23f\n");
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
//Last line
