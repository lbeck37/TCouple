const char szSketchName[]  = "B32_FirstFreeRTOS.ino";
const char szFileDate[]    = "12/8/23b";

//#include <Streaming.h>

#define TASK1_PERIOD_SEC	   2
#define TASK2_PERIOD_SEC	   1
#define ONE_SECOND_DELAY	1000

//Function protos
void      LaunchTasks   (void);
void 		  Task1		      (void *pvParameter);
void 		  Task2		      (void *pvParameter);
//int64_t 	lGetMsec	    (void);

void setup() {
  Serial.begin(115200);
  Serial.print("LaunchTasks(): Begin ");
  Serial.println(szFileDate);

  LaunchTasks();
  return;
} //setup

void loop() {
//We do nothing in loop since the tasks are running
return;
} //loop

void LaunchTasks(void){	
	// cout << endl << lGetMsec() << " LaunchTasks(): File: " << szFileName << ", " << szFileDate << endl;
	// cout << lGetMsec() << " LaunchTasks(): Launch Task1 and Task2" << endl;
  Serial.println("LaunchTasks(): Begin");
	xTaskCreate(&Task1, "Task_One", 2048, NULL, 5, NULL);
	xTaskCreate(&Task2, "Task_Two", 2048, NULL, 5, NULL);
	while(true){
		vTaskDelay(ONE_SECOND_DELAY/portTICK_PERIOD_MS);	//Not sure if one second is good
	}	//while(true)
	return;
}	//LaunchTasks


void Task1(void *pvParameter){
	while(true){
		//cout << lGetMsec() << " Task1(): Hello World!" << endl;
    Serial.println("Task1(): Hello World!");

		vTaskDelay((TASK1_PERIOD_SEC * ONE_SECOND_DELAY)/portTICK_PERIOD_MS);
	}	//while(true)
  //It's a task so it should never return
}   //Task1


void Task2(void *pvParameter){
	while(true){
		//cout << lGetMsec() << " Task2(): It's my turn!" << endl;
    Serial.println("Task2(): It's my turn!");

		vTaskDelay((TASK2_PERIOD_SEC * ONE_SECOND_DELAY)/portTICK_PERIOD_MS);
	}	//while(true)
  //It's a task so it should never return
}   //Task2


// int64_t lGetMsec(void){
// 	// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html
// 	struct timeval 		stTimeNow;
// 	int64_t 			lTimeMsec;

// 	gettimeofday(&stTimeNow, NULL);
// 	lTimeMsec = ((int64_t)stTimeNow.tv_sec * 1000L) + ((int64_t)stTimeNow.tv_usec / 1000L);
// 	return lTimeMsec;
// }	//lGetMsec
//Last line