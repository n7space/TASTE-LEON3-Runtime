#ifndef INTERFACES_INFO_H
#define INTERFACES_INFO_H

#define MAX_THREAD_NAME_SIZE 32
#define RUNTIME_THREAD_COUNT 8
#define TIMER_SCALER_VALUE 60

typedef struct {
	int id;
	int min_thread_execution_time;
	int max_thread_execution_time;
	int mean_thread_execution_time;
	char *name;
} thread_info;

enum interfaces_enum { INTERFACE_MOCK };

thread_info threads_info[RUNTIME_THREAD_COUNT];

#endif