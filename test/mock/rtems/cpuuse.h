#ifndef CPUUSE_H
#define CPUUSE_H

#include <stddef.h>
#include <stdbool.h>

#define TOD_MICROSECONDS_PER_SECOND (int)1000000
#define TOD_NANOSECONDS_PER_MICROSECOND (int)1000

typedef int Timestamp_Control;

typedef struct {
	int id;
} Object_MOCK;

typedef struct {
	Object_MOCK Object;
} Thread_Control;

typedef Thread_Control rtems_tcb;

typedef bool (*rtems_task_visitor)(rtems_tcb *, void *);

void rtems_cpu_usage_reset() {}

void _TOD_Get_uptime(Timestamp_Control *time) {}

void _Timestamp_Set_to_zero(Timestamp_Control *_time) {}

Timestamp_Control
_Thread_Get_CPU_time_used_after_last_reset(Thread_Control *the_thread)
{ return 0; }

size_t _Thread_Get_name(const Thread_Control *the_thread, char *buffer,
			size_t buffer_size)
{ return 0; }

void _Timestamp_Subtract(const Timestamp_Control *_start,
			 const Timestamp_Control *_end,
			 Timestamp_Control *_result)
{
}

void _Timestamp_Divide(const Timestamp_Control *_lhs,
		       const Timestamp_Control *_rhs,
		       unsigned int *_ival_percentage, unsigned int *_fval_percentage)
{
}

void rtems_task_iterate(rtems_task_visitor visitor, void *arg) {}

#endif