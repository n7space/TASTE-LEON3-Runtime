#ifndef INTERFACES_INFO_H
#define INTERFACES_INFO_H

#include <stdint.h>
#include <rtems.h>

#define MAX_THREAD_NAME_SIZE 32
#define RUNTIME_THREAD_COUNT 8
#define TIMER_SCALER_VALUE 60


typedef struct {
	uint32_t id;
	uint64_t min_thread_execution_time;
	uint64_t max_thread_execution_time;
	uint64_t mean_thread_execution_time;
	uint8_t *name;
	uint64_t thread_execution_time;
	uint64_t execution_time_counter;
} thread_info;

enum interfaces_enum { INTERFACE_MOCK };

extern thread_info threads_info[RUNTIME_THREAD_COUNT];

extern rtems_id interface_to_queue_map[RUNTIME_THREAD_COUNT];

extern uint32_t maximum_queued_items[RUNTIME_THREAD_COUNT];

#endif