/**@file
 * This file is part of the TASTE Leon3 Runtime.
 *
 * @copyright 2022 N7 Space Sp. z o.o.
 *
 * TASTE Leon3 Runtime was developed under the project AURORA.
 * This project has received funding from the European Union’s Horizon 2020
 * research and innovation programme under grant agreement No 101004291
 *
 * Leon3 Runtime is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with Leon3 Runtime. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Perfmon.h>

static thread_info Perf_Mon_getThreadInfo(Perf_Mon *const perf_mon_ctx,
					  const uint64_t threadId)
{
	thread_info empty_info = {};

	for (int i = 0; i < RUNTIME_THREAD_COUNT; i++) {
		if (threadId == threads_info[i].id) {
			return threads_info[i];
		}
	}

	return empty_info;
}

static bool Perf_Mon_cpuUsageVisitor(Thread_Control *the_thread, void *arg)
{
	Perf_Mon *const context = (Perf_Mon *)arg;
	char thread_name[MAX_THREAD_NAME_SIZE];
	float usage_percent;
	uint32_t integer_val;
	uint32_t float_val;
	Timestamp_Control uptime;
	const Timestamp_Control used_time =
	    _Thread_Get_CPU_time_used_after_last_reset(the_thread);

	_Thread_Get_name(the_thread, thread_name, sizeof(thread_name));

	_TOD_Get_uptime(&uptime);
	_Timestamp_Subtract(&context->uptime_at_last_reset, &uptime,
			    &context->total_usage_time);
	_Timestamp_Divide(&used_time, &context->total_usage_time, &integer_val,
			  &float_val);

	usage_percent = (float)float_val / TOD_NANOSECONDS_PER_MICROSECOND;
	usage_percent += (float)integer_val;

	if (usage_percent <
	    context->usage_data[context->task_iterator].cpu_usage_min_val) {
		context->usage_data[context->task_iterator].cpu_usage_min_val =
		    usage_percent;
	}

	if (usage_percent >
	    context->usage_data[context->task_iterator].cpu_usage_max_val) {
		context->usage_data[context->task_iterator].cpu_usage_max_val =
		    usage_percent;
	}

	const float mean_val =
	    context->usage_data[context->task_iterator].cpu_usage_mean_val +
	    (usage_percent -
	     context->usage_data[context->task_iterator].cpu_usage_mean_val) /
		(context->benchmarking_ticks + 1);

	const thread_info thread_info_data =
	    Perf_Mon_getThreadInfo(context, the_thread->Object.id);

	context->usage_data[context->task_iterator].cpu_usage_mean_val =
	    mean_val;
	context->usage_data[context->task_iterator]
	    .system_ticks_spent_by_interface_min_val =
	    thread_info_data.min_thread_execution_time;
	context->usage_data[context->task_iterator]
	    .system_ticks_spent_by_interface_max_val =
	    thread_info_data.max_thread_execution_time;
	context->usage_data[context->task_iterator]
	    .system_ticks_spent_by_interface_mean_val =
	    thread_info_data.mean_thread_execution_time;

	context->task_iterator++;
	return false;
}

static bool Perf_Mon_dataInitVisitor(Thread_Control *the_thread, void *arg)
{
	Perf_Mon *const context = (Perf_Mon *)arg;
	char name[MAX_THREAD_NAME_SIZE];

	_Thread_Get_name(the_thread, name, sizeof(name));

	context->usage_data[context->task_iterator].id = the_thread->Object.id;
	strncpy(context->usage_data[context->task_iterator].thread_name, name,
		sizeof(name));
	context->usage_data[context->task_iterator]
	    .thread_name[sizeof(name) - 1] = '\0';
	context->usage_data[context->task_iterator].cpu_usage_min_val = FLT_MAX;
	context->usage_data[context->task_iterator]
	    .system_ticks_spent_by_interface_min_val = (uint64_t)(-1);

	context->task_iterator++;
	context->task_count++;
	return false;
}

void Perf_Mon_init(Perf_Mon *const perf_mon_ctx)
{
	rtems_cpu_usage_reset();
	_TOD_Get_uptime(&perf_mon_ctx->uptime_at_last_reset);
	perf_mon_ctx->is_initialized = false;
}

void Perf_Mon_tick(Perf_Mon *const perf_mon_ctx)
{
	if (!perf_mon_ctx->is_initialized) {
		perf_mon_ctx->task_count = 0;
		perf_mon_ctx->task_iterator = 0;
		perf_mon_ctx->benchmarking_ticks = 0;

		rtems_task_iterate(Perf_Mon_dataInitVisitor, perf_mon_ctx);

		perf_mon_ctx->is_initialized = true;
	}

	_Timestamp_Set_to_zero(&perf_mon_ctx->total_usage_time);
	perf_mon_ctx->task_iterator = 0;

	rtems_task_iterate(Perf_Mon_cpuUsageVisitor, perf_mon_ctx);
	perf_mon_ctx->benchmarking_ticks++;
}

Interface_Usage_Data Perf_Mon_getUsageData(Perf_Mon *const perf_mon_ctx,
					   const enum interfaces_enum interface,
					   const double system_frequency)
{
	Interface_Usage_Data data;
	rtems_id id = threads_info[interface].id;

	for (int i = 0; i < perf_mon_ctx->task_count; i++) {
		if (id == perf_mon_ctx->usage_data[i].id) {
			data = perf_mon_ctx->usage_data[i];
			strncpy(data.interface_name,
				threads_info[interface].name,
				sizeof(data.interface_name));
			data.interface_name[sizeof(data.interface_name) - 1] =
			    '\0';

			data.microseconds_spent_by_interface_min_val =
			    (double)(data.system_ticks_spent_by_interface_min_val) *
			    (double)(TIMER_SCALER_VALUE) *
			    (double)(TOD_MICROSECONDS_PER_SECOND) /
			    system_frequency;

			data.microseconds_spent_by_interface_max_val =
			    (double)(data.system_ticks_spent_by_interface_max_val) *
			    (double)(TIMER_SCALER_VALUE) *
			    (double)(TOD_MICROSECONDS_PER_SECOND) /
			    system_frequency;

			data.microseconds_spent_by_interface_mean_val =
			    (double)(data.system_ticks_spent_by_interface_mean_val) *
			    (double)(TIMER_SCALER_VALUE) *
			    (double)(TOD_MICROSECONDS_PER_SECOND) /
			    system_frequency;
			break;
		}
	}

	return data;
}

Summarized_Usage_Data Perf_Mon_getIdleUsageData(Perf_Mon *const perf_mon_ctx)
{
	Summarized_Usage_Data data;
	Interface_Usage_Data usageData;

	usageData = perf_mon_ctx->usage_data[0];

	strncpy(data.name, "IDLE", sizeof(data.name));
	data.cpu_usage_min_val = usageData.cpu_usage_min_val;
	data.cpu_usage_max_val = usageData.cpu_usage_max_val;
	data.cpu_usage_mean_val = usageData.cpu_usage_mean_val;

	return data;
}

Summarized_Usage_Data
Perf_Mon_getThreadsSummarizedUsageData(Perf_Mon *const perf_mon_ctx)
{
	Summarized_Usage_Data data;
	Interface_Usage_Data usageData;

	usageData = perf_mon_ctx->usage_data[0];

	strncpy(data.name, "SUMMARIZED", sizeof(data.name));
	data.cpu_usage_min_val = 100.0 - usageData.cpu_usage_min_val;
	data.cpu_usage_max_val = 100.0 - usageData.cpu_usage_max_val;
	data.cpu_usage_mean_val = 100.0 - usageData.cpu_usage_mean_val;

	return data;
}