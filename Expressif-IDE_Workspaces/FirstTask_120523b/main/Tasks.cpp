const char szFileName[]  = "Tasks.cpp";
const char szFileDate[]  = "12/5/23c";

#include <TFT_eSPI.h>

#include "Tasks.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

//#include <Streaming.h>
#include <iostream>
using std::cout;
using std::endl;

#define TASK1_PERIOD_SEC	   2
#define TASK2_PERIOD_SEC	   1
#define ONE_SECOND_DELAY	1000

//Function protos
void 		Task1		(void *pvParameter);
void 		Task2		(void *pvParameter);
int64_t 	lGetMsec	(void);

#ifdef __cplusplus
	extern "C" {
#endif

void LaunchTasks(void *pvParameter){	//Prototype in Tasks.h
	cout << endl << lGetMsec() << " LaunchTasks(): File: " << szFileName << ", " << szFileDate << endl;
	cout << lGetMsec() << " LaunchTasks(): Launch Task1 and Task2" << endl;
	xTaskCreate(&Task1, "Task_One", 2048, NULL, 5, NULL);
	xTaskCreate(&Task2, "Task_Two", 2048, NULL, 5, NULL);
	while(true){
		vTaskDelay(ONE_SECOND_DELAY/portTICK_PERIOD_MS);	//Not sure if one second is good
	}	//while(true)
	//Never returns
}	//LaunchTasks

#ifdef __cplusplus
	}
#endif


//Doesn't need extern "C" surround since it's not called from C code (main.c)
void Task1(void *pvParameter){
	while(true){
		cout << lGetMsec() << " Task1(): Hello World!" << endl;

		vTaskDelay((TASK1_PERIOD_SEC * ONE_SECOND_DELAY)/portTICK_PERIOD_MS);
	}	//while(true)
  //It's a task so it should never return
}   //Task1


//Doesn't need extern "C" surround since it's not called from C code (main.c)
void Task2(void *pvParameter){
	while(true){
		cout << lGetMsec() << " Task2(): It's my turn!" << endl;
		vTaskDelay((TASK2_PERIOD_SEC * ONE_SECOND_DELAY)/portTICK_PERIOD_MS);
	}	//while(true)
  //It's a task so it should never return
}   //Task2


int64_t lGetMsec(void){
	// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html
	struct timeval 		stTimeNow;
	int64_t 			lTimeMsec;

	gettimeofday(&stTimeNow, NULL);
	lTimeMsec = ((int64_t)stTimeNow.tv_sec * 1000L) + ((int64_t)stTimeNow.tv_usec / 1000L);
	return lTimeMsec;
}	//lGetMsec
//Last line
