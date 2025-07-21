/**@file
 * This file is part of the TASTE Leon3 Runtime.
 *
 * @copyright 2025 N7 Space Sp. z o.o.
 *
 * Leon3 Runtime is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with Leon3 Runtime. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Hal.h>
#include <ThreadsCommon.h>
#include <interfaces_info.h>
#include <rtems.h>

#ifndef RT_MAX_CYCLIC_INTERFACES
#define RT_MAX_CYCLIC_INTERFACES 8
#endif

#define NANOSECOND_IN_MILISECOND 1000000

extern rtems_name generate_new_partition_timer_name();
static void ScheduleNextTick(rtems_id timer_id);
static void TimerCallback(rtems_id timer_id, void *unused);
static void update_execution_time_data(uint32_t thread_id,
				       const uint64_t thread_execution_time);

typedef void (*call_function)(const char *buf, size_t len);

struct CyclicRequestData {
	rtems_id timer_id;
	rtems_interval next_wakeup_ticks;
	rtems_interval interval_ticks;
	uint32_t queue_id;
	uint32_t request_size;
	bool is_used;
};

struct CyclicEmptyRequestData {
	uint32_t m_sender_pid;
	uint32_t m_length;
	uint8_t *m_data __attribute__((aligned(16)));
};

static struct CyclicRequestData cyclic_reqeust_data[RT_MAX_CYCLIC_INTERFACES];
static struct CyclicEmptyRequestData empty_request;

static void ScheduleNextTick(rtems_id timer_id)
{
	uint32_t index = 0;
	for (uint32_t index = 0; index < RT_MAX_CYCLIC_INTERFACES; index++) {
		if (!cyclic_reqeust_data[index].is_used &&
		    cyclic_reqeust_data[index].timer_id == timer_id) {
			break;
		}
	}

	cyclic_reqeust_data[index].next_wakeup_ticks +=
	    cyclic_reqeust_data[index].interval_ticks;
	rtems_interval delta = cyclic_reqeust_data[index].next_wakeup_ticks -
			       rtems_clock_get_ticks_since_boot();
	rtems_timer_fire_after(timer_id, delta, TimerCallback, NULL);
}

static void TimerCallback(rtems_id timer_id, void *unused)
{
	uint32_t index = 0;
	for (uint32_t index = 0; index < RT_MAX_CYCLIC_INTERFACES; index++) {
		if (!cyclic_reqeust_data[index].is_used &&
		    cyclic_reqeust_data[index].timer_id == timer_id) {
			break;
		}
	}

	rtems_message_queue_send(cyclic_reqeust_data[index].queue_id,
				 &empty_request,
				 cyclic_reqeust_data[index].request_size);
	ScheduleNextTick(cyclic_reqeust_data[index].timer_id);
}

static void update_execution_time_data(uint32_t thread_id,
				       const uint64_t thread_execution_time)
{

	if (thread_execution_time <
	    threads_info[thread_id].min_thread_execution_time) {
		threads_info[thread_id].min_thread_execution_time =
		    thread_execution_time;
	}

	if (thread_execution_time >
	    threads_info[thread_id].max_thread_execution_time) {
		threads_info[thread_id].max_thread_execution_time =
		    thread_execution_time;
	}

	threads_info[thread_id].mean_thread_execution_time =
	    threads_info[thread_id].mean_thread_execution_time +
	    ((double)thread_execution_time -
	     threads_info[thread_id].mean_thread_execution_time) /
		((double)threads_info[thread_id].execution_time_counter + 1);

	threads_info[thread_id].execution_time_counter++;
}

bool ThreadsCommon_CreateCyclicRequest(uint64_t interval_ns, uint32_t queue_id,
				       uint32_t request_size)
{
	bool empty_slot_found = false;
	uint32_t index = 0;

	for (index = 0; index < RT_MAX_CYCLIC_INTERFACES; index++) {
		if (!cyclic_reqeust_data[index].is_used) {
			empty_slot_found = true;
		}
	}

	if (!empty_slot_found) {
		return false;
	}

	rtems_name name = generate_new_partition_timer_name();
	const rtems_status_code timer_creation_status =
	    rtems_timer_create(name, &cyclic_reqeust_data[index].timer_id);
	if (timer_creation_status != RTEMS_SUCCESSFUL) {
		return false;
	}

	cyclic_reqeust_data[index].next_wakeup_ticks = 0;
	cyclic_reqeust_data[index].interval_ticks =
	    RTEMS_MILLISECONDS_TO_TICKS(interval_ns * NANOSECOND_IN_MILISECOND);
	cyclic_reqeust_data[index].queue_id = queue_id;
	cyclic_reqeust_data[index].request_size = request_size;
	cyclic_reqeust_data[index].is_used = true;

	ScheduleNextTick(cyclic_reqeust_data[index].timer_id);

	return true;
}

bool ThreadsCommon_ProcessRequest(void *request_data, uint32_t request_size,
				  void *user_function, uint32_t thread_id)
{
	call_function casted_user_function = user_function;

	const uint64_t time_before_execution = Hal_GetElapsedTimeInNs();
	casted_user_function((const char *)request_data, request_size);
	const uint64_t time_after_execution = Hal_GetElapsedTimeInNs();

	threads_info[thread_id].thread_execution_time =
	    time_before_execution - time_after_execution;
	update_execution_time_data(
	    thread_id, threads_info[thread_id].thread_execution_time);

	return true;
}