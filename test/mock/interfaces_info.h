#ifndef INTERFACES_INFO_H
#define INTERFACES_INFO_H

#include <stdint.h>

#define MAX_THREAD_NAME_SIZE 32
#define RUNTIME_THREAD_COUNT 8
#define TIMER_SCALER_VALUE 60


typedef struct {
	int id;
	uint64_t min_thread_execution_time;
	uint64_t max_thread_execution_time;
	uint64_t mean_thread_execution_time;
	char *name;
	uint64_t thread_execution_time;
	uint64_t execution_time_counter;
} thread_info;

enum interfaces_enum { INTERFACE_MOCK };

thread_info threads_info[RUNTIME_THREAD_COUNT];

#endif