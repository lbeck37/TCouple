//Beck, Tasks.h, 12/2/23e
#pragma once

#if 0
#ifdef __cplusplus
	extern "C" {
#endif

void Task1		(void *pvParameter);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
	extern "C" {
#endif

void Task2		(void *pvParameter);

#ifdef __cplusplus
}
#endif
#else
	#ifdef __cplusplus
		extern "C" {
	#endif
		void LaunchTasks	(void *pvParameter);
	#ifdef __cplusplus
	}
#endif
#endif
//Last line.
