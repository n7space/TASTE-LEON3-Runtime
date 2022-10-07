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

#ifndef PERFMON_H
#define PERFMON_H

/**
 * @file    Perfmon.h
 * @brief   Header for performance monitoring of TASTE interfaces
 */

#include "interfaces_info.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <rtems.h>
#include <rtems/cpuuse.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/todimpl.h>

/**
 * @brief   Struct representing usage and benchmarking data for the given interface
 */
typedef struct 
{
    uint32_t          id;
    char              thread_name[MAX_THREAD_NAME_SIZE];
    char              interface_name[MAX_THREAD_NAME_SIZE];
    float             cpu_usage_min_val;
    float             cpu_usage_max_val;
    float             cpu_usage_mean_val;
    uint64_t          system_ticks_spent_by_interface_min_val;
    uint64_t          system_ticks_spent_by_interface_max_val;
    double            system_ticks_spent_by_interface_mean_val;
    uint64_t          microseconds_spent_by_interface_min_val;
    uint64_t          microseconds_spent_by_interface_max_val;
    double            microseconds_spent_by_interface_mean_val;
} Interface_Usage_Data;

/**
 * @brief   Struct representing usage data for idle and
 *          all threads summed up states
 */
typedef struct 
{
    char              name[MAX_THREAD_NAME_SIZE];
    float             cpu_usage_min_val;
    float             cpu_usage_max_val;
    float             cpu_usage_mean_val;
} Summarized_Usage_Data;

/**
 * @brief   Struct representing Perfmon context
 */
typedef struct 
{
    Interface_Usage_Data     usage_data[RUNTIME_THREAD_COUNT];
    uint32_t                 task_iterator;
    uint32_t                 task_count;
    uint64_t                 benchmarking_ticks;
    bool                     is_initialized;
    Timestamp_Control        total_usage_time;
    Timestamp_Control        uptime_at_last_reset;
} Perf_Mon;

/**
 * @brief   Returns thread information object
 *
 * @param[in] perf_mon_ctx     Perfmon context
 * @param[in] threadId         Id of the thread
 *
 * @return  Thread information object
 */
static thread_info Perf_Mon_getThreadInfo(Perf_Mon *const perf_mon_ctx, const uint64_t threadId);

/**
 * @brief   Visitor, updates benchmarking data of the thread,
 *          check rtems_task_iterate
 *
 * @param[in] the_thread       Thread for benchmarking data extraction
 * @param[in] arg              Argument pointer, should be Perfmon context
 *
 * @return  Returns false
 */
static bool Perf_Mon_cpuUsageVisitor(Thread_Control *the_thread, void *arg);

/**
 * @brief   Visitor, initialize benchmarking data of the thread,
 *          check rtems_task_iterate
 *
 * @param[in] the_thread       Thread for benchmarking data extraction
 * @param[in] arg              Argument pointer, should be Perfmon context
 *
 * @return  Returns false
 */
static bool Perf_Mon_dataInitVisitor(Thread_Control *the_thread, void *arg);

/**
 * @brief   Perfmon initializer, should be invoked on system startup
 *
 * @param[in] perf_mon_ctx     Perfmon context
 */
void Perf_Mon_init(Perf_Mon *const perf_mon_ctx);

/**
 * @brief   Perfmon tick, updates interfaces benchmarking information data
 *          should be invoked in benchmarking thread loop
 *
 * @param[in] perf_mon_ctx     Perfmon context
 */
void Perf_Mon_tick(Perf_Mon *const perf_mon_ctx);

/**
 * @brief   Gets benchmarking information data for a given thread
 *
 * @param[in] perf_mon_ctx       Perfmon context
 * @param[in] interface          Enum representing given interface, check interfaces_info.h
 * @param[in] system_frequency   system frequency given in Hz
 * 
 * @return  Returns benchmarking information data
 */
Interface_Usage_Data Perf_Mon_getUsageData(Perf_Mon *const perf_mon_ctx, 
                                           const enum interfaces_enum interface, 
                                           const double system_frequency);

/**
 * @brief   Gets benchmarking information data for Idle state
 *
 * @param[in] perf_mon_ctx       Perfmon context
 * 
 * @return  Returns benchmarking information data for Idle state
 */
Summarized_Usage_Data Perf_Mon_getIdleUsageData(Perf_Mon *const perf_mon_ctx);

/**
 * @brief   Gets benchmarking information data for all interfaces summarized
 *
 * @param[in] perf_mon_ctx       Perfmon context
 * 
 * @return  Returns benchmarking information data for all interfaces
 */
Summarized_Usage_Data Perf_Mon_getThreadsSummarizedUsageData(Perf_Mon *const perf_mon_ctx);

#endif